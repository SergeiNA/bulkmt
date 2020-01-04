/**
 * @file observer.h
 * @author SergeiNA (you@domain.com)
 * @brief observer pattern implementation
 * 
 * log_observer - logging packages info to files
 * terminal_observer - display packages to stdout
 * create - initialize file or display "bulk"
 * end - close file or add endl
 * bukl - dump package of commands
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <atomic>

typedef std::chrono::nanoseconds rawTimestamp;
typedef std::pair<std::vector<std::string>,rawTimestamp> Packet;
/**
 * @brief Observer interface
 * 
 */
class Observer {
public:
    virtual void bulk(const Packet &) = 0;
    virtual ~Observer() =default;
};
/**
 * @brief Display packages to stdout
 * 
 * ex: bulk: cmd1,cmd2
 *     bulk: cmd3
 */
class terminal_observer : public Observer {
public:
    terminal_observer(std::ostream& ofs = std::cout);
    /**
     * @brief display command to stdout
     * 
     * @param cmd command to display
     */
    void bulk(const Packet& pack) override;
private:
    bool isBegin; ///< indicate start of printing to stdout
    std::ostream& os;
};


/**
 * @brief Create and wtite to file package of commands
 * name files as bulk<unixtime>.log
 */
class log_observer : public Observer {
public:
    log_observer();
    /**
     * @brief write commands to file
     * 
     * @param cmd written command
     */
    void bulk(const Packet& pack) override;
private:
    static std::atomic_int id;
    std::ofstream File_;
};


