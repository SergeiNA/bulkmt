#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <future>
#include <memory>

#include "metrics.h"

class ThreadManager{

public:
    void subscribe(std::shared_ptr<Observer> obs, std::shared_ptr<Metrics> metrics);
    void push(Packet packet);
    void stop();
    ~ThreadManager();

private:
    std::atomic_bool quit = false;
    std::condition_variable conditional_v;
    std::mutex conditional_mutex;
    std::vector<std::thread> subs;
    std::queue<Packet> packetQueue;
};