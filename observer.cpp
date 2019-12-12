/**
 * @file observer.cpp
 * @author SergeiNA (you@domain.com)
 * @brief log_observer and terminal_observer method definition
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "observer.h"

terminal_observer::terminal_observer(std::ostream& ofs):
    isBegin(true),os(ofs) {}
//string_view
void terminal_observer::bulk(const std::pair<std::vector<std::string>,rawTimestamp>& pack)  {
    os << "bulk: ";
    for(const auto& c: pack.first){
        if (!isBegin)
            os << ", ";
        isBegin = false;
        os << c;
    }
    os << std::endl;
    isBegin =true;
}

log_observer::log_observer() {}
//string_view
void log_observer::bulk(const std::pair<std::vector<std::string>,rawTimestamp>& pack){
    using namespace std::chrono;
    File_.open("bulk" + std::to_string(duration_cast<milliseconds>(pack.second).count()) + ".log");
    for(const auto& c:pack.first)
        File_ << c << std::endl;
    File_.close();
}
