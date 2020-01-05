/**
 * @file bulkmt.cpp
 * @author SergeiNA (you@domain.com)
 * @brief OTUS homework number 09
 * 
 * package handler
 * @version 1.0
 * @date 2020-01-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "command_handler.h"

std::atomic_int log_observer::id=0;

int main([[maybe_unused]]int argc, char* argv[]){

    auto fileLogMetrics2 = std::make_shared<SideMetrics>("File2");
    auto fileLogMetrics1 = std::make_shared<SideMetrics>("File1");
    auto termLogMetrics = std::make_shared<SideMetrics>("Log");
    auto mainMetrics = std::make_shared<MainMetrics>();

    auto fileLogThreadManager = std::make_shared<ThreadManager>();
    auto terminalLogThreadManager = std::make_shared<ThreadManager>();

    fileLogThreadManager->subscribe(std::make_shared<log_observer>(),fileLogMetrics1);
    fileLogThreadManager->subscribe(std::make_shared<log_observer>(),fileLogMetrics2);
    terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(),termLogMetrics);

    auto q_command = std::make_unique<QueueCommand>(atoi(argv[1]),mainMetrics);

    q_command->subscribe(fileLogThreadManager);
    q_command->subscribe(terminalLogThreadManager);

	CommandHandler cmdHandler(std::move(q_command),mainMetrics);
	cmdHandler.Run(std::cin);

    return 0;
}