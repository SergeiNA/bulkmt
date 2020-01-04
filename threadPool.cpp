#include "threadPool.h"

std::mutex console_m;

void taskProcess(std::queue<Packet>& packetQueue,
            std::condition_variable& conditional_v,
            std::mutex& conditional_mutex,
            const std::atomic_bool& quit,
            std::function<void(const Packet&)> bulk)
{
    while (true) {

        std::unique_lock<std::mutex> lk(conditional_mutex);

        console_m.lock();
        std::cerr << std::this_thread::get_id() << " waiting... " << std::endl;
        console_m.unlock();

        conditional_v.wait(lk, [&packetQueue,&quit]() { return !packetQueue.empty() || quit; });

        if (packetQueue.empty() && quit)
        {

            console_m.lock();
            std::cerr << std::this_thread::get_id() << " Return " << std::endl;
            console_m.unlock();

            return;
        }

        if (!packetQueue.empty()) {
            auto packet = std::move(packetQueue.front());
            packetQueue.pop();
            auto s = packetQueue.size();
            lk.unlock();

            bulk(packet);

            console_m.lock();
            std::cerr << std::this_thread::get_id() << " leave " << s << std::endl;
            console_m.unlock();
        }
    }
    console_m.lock();
    std::cerr << std::this_thread::get_id() << " End... " << std::endl;
    console_m.unlock();
}



void TaskManager::subscribe(std::shared_ptr<Observer> obs){
    subs.emplace_back(std::thread(taskProcess,
                      std::ref(packetQueue),
                      std::ref(conditional_v),
                      std::ref(conditional_mutex),
                      std::cref(quit),
                      [obs](const Packet& p){
                          obs->bulk(p);
                      }));
}

void TaskManager::push(Packet packet){
    std::unique_lock<std::mutex> lk(conditional_mutex);
    packetQueue.emplace(packet);
    console_m.lock();
    std::cerr << "Insert size packetQueue: " << packetQueue.size() << std::endl;
    console_m.unlock();
    lk.unlock();

    conditional_v.notify_all();
}

void TaskManager::stop(){
    conditional_v.notify_all();
    if(!quit)
        quit=true;
}

TaskManager::~TaskManager(){
    console_m.lock();
    std::cerr << "Delete" << std::endl;
    console_m.unlock();

    TaskManager::stop();
    for(auto&& s:subs)
        if(s.joinable())
            s.join();
}
