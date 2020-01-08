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

int main([[maybe_unused]]int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Wrong input: Bulkmt can take only one parameter: bulk size" << std::endl;
        return 1;
    }

    size_t bulkSize =1;

    try {
        bulkSize = std::stol(argv[1]);

    } catch (const std::exception& e) {
        std::cerr << "'" << argv[1] << "'" 
            << " -  NaN. Bulk size can be only a number"<< std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    /// create metric collectors
    auto fileLogMetrics2 = std::make_shared<SideMetrics>("File2");
    auto fileLogMetrics1 = std::make_shared<SideMetrics>("File1");
    auto termLogMetrics = std::make_shared<SideMetrics>("Log");
    auto mainMetrics = std::make_shared<MainMetrics>();

    /// create threadManager for each types of loggers
    auto fileLogThreadManager = std::make_shared<ThreadManager>();
    auto terminalLogThreadManager = std::make_shared<ThreadManager>();

    /// create and subscribe loggers to corresponding threadManagers
    fileLogThreadManager->subscribe(std::make_shared<log_observer>(),fileLogMetrics1);
    fileLogThreadManager->subscribe(std::make_shared<log_observer>(),fileLogMetrics2);
    terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(),termLogMetrics);

    /// create QueueCommnad with mainMetric
    auto q_command = std::make_unique<QueueCommand>(bulkSize,mainMetrics);

    /// subscribe threadManagers to QueueCommnad
    q_command->subscribe(fileLogThreadManager);
    q_command->subscribe(terminalLogThreadManager);

    /// create CommandHandler with prepared QueueCommnad and also add mainMetrics collector
	CommandHandler cmdHandler(std::move(q_command),mainMetrics);
    
    try
    {
        cmdHandler.Run(std::cin);
    }
    catch(const std::exception& e)
    {
        std::cerr<< "CommandHandler error: " << e.what() << '\n';
    }
    

    return 0;
    /// at this point metric collectors starting 
    /// to destroy and in distructors call display methods
}