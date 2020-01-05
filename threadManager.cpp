#include "threadManager.h"

// std::mutex console_m;

void taskProcess(std::queue<Packet>& packetQueue,
            std::condition_variable& conditional_v,
            std::mutex& conditional_mutex,
            const std::atomic_bool& quit,
            std::function<void(const Packet&)> bulk)
{
    while (true) {

        std::unique_lock<std::mutex> lk(conditional_mutex);

        // console_m.lock();
        // std::cerr << std::this_thread::get_id() << " waiting... " << std::endl;
        // console_m.unlock();

        conditional_v.wait(lk, [&packetQueue,&quit]() { return !packetQueue.empty() || quit; });

        if (packetQueue.empty() && quit)
        {

            // console_m.lock();
            // std::cerr << std::this_thread::get_id() << " Return " << std::endl;
            // console_m.unlock();

            return;
        }

        if (!packetQueue.empty()) {
            auto packet = std::move(packetQueue.front());
            packetQueue.pop();
            // auto s = packetQueue.size();
            lk.unlock();

            bulk(packet);

            // console_m.lock();
            // std::cerr << std::this_thread::get_id() << " leave " << s << std::endl;
            // console_m.unlock();
        }
    }
    // console_m.lock();
    // std::cerr << std::this_thread::get_id() << " End... " << std::endl;
    // console_m.unlock();
}



void ThreadManager::subscribe(std::shared_ptr<Observer> obs,  std::shared_ptr<Metrics> metrics){
    subs.emplace_back(std::thread(taskProcess,
                      std::ref(packetQueue),
                      std::ref(conditional_v),
                      std::ref(conditional_mutex),
                      std::cref(quit),
                      [obs, metrics](const Packet& p){
                          metrics->collect(p.first);
                          obs->bulk(p);
                      }));
}

void ThreadManager::push(Packet packet){
    std::unique_lock<std::mutex> lk(conditional_mutex);
    packetQueue.emplace(packet);
    lk.unlock();
    conditional_v.notify_all();
}

void ThreadManager::stop(){
    conditional_v.notify_all();
    if(!quit)
        quit=true;
}

ThreadManager::~ThreadManager(){

    // console_m.lock();
    // std::cerr << "Delete" << std::endl;
    // console_m.unlock();

    ThreadManager::stop();
    for(auto&& s:subs)
        if(s.joinable())
            s.join();
}
