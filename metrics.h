#pragma onec
#include "observer.h"

#include <iostream>
#include <string>

class Metrics{
public:
    virtual void push(const Packet& packet, size_t nstr)=0;
    virtual void display(size_t ID) const =0;
};


class MainMetrics : public Metrics{
public:
    MainMetrics():n_str{0},n_cmds{0},n_blocks{0} {}
    void push(const Packet& pack, size_t nstr) override{
        n_str+=nstr;
        ++n_blocks;
        n_cmds+=pack.first.size();
    }
    void display(size_t ID) const override{
        std::cout<<"Main thread ID: "<<ID<<" - "
        <<n_str<<" strings, "<<n_cmds<<" commands, "
        <<n_blocks<<" blocks"<<std::endl;
    }
private:
    size_t n_str;
    size_t n_cmds;
    size_t n_blocks;
};

class SideMetrics : public Metrics{
public:
    SideMetrics(std::string name):name_{name},n_cmds{0},n_blocks{0} {}
    void push(const Packet& pack, [[maybe_unused]]size_t nstr=0) override{
        ++n_blocks;
        n_cmds+=pack.first.size();
    }
    void display(size_t ID) const override{
        std::cout<<name_<<" thread ID: "<<ID<<" - "
        <<n_blocks<<" blocks, "
        <<n_cmds<<" commands"<<std::endl;
    }
private:
    std::string name_;
    size_t n_cmds;
    size_t n_blocks;
};