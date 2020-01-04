#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <future>
#include <memory>

#include "observer.h"


// void workers(std::queue<Packet>& packetQueue,
//             std::condition_variable& conditional_v,
//             std::mutex& conditional_mutex,
//             const std::atomic_bool& quit,
//             std::function<void(const Packet&)> bulk);


class TaskManager{

public:
    void subscribe(std::shared_ptr<Observer> obs);

    void push(Packet packet);
    void stop();
    ~TaskManager();

private:
    std::atomic_bool quit = false;
    std::condition_variable conditional_v;
    std::mutex conditional_mutex;
    std::vector<std::thread> subs;
    std::queue<Packet> packetQueue;
};