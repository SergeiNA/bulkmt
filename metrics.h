#pragma once
#include "observer.h"


class Metrics{
public:
    virtual void collect(const std::vector<std::string>& packet, size_t nstr =0)=0;
    virtual void display() const =0;
    virtual ~Metrics()=default;
};


class MainMetrics : public Metrics{
public:
    MainMetrics(std::ostream& ofs = std::cout);

    void collect(const std::vector<std::string>& pack, size_t nstr) override;
    void display() const override;
    ~MainMetrics();
    
private:
    std::ostream& os;
    size_t n_str;
    size_t n_cmds;
    size_t n_blocks;
};

class SideMetrics : public Metrics{
public:
    SideMetrics(std::string name, std::ostream& ofs = std::cout);

    void collect(const std::vector<std::string>& pack, [[maybe_unused]]size_t nstr=0) override;
    void display() const override;

    ~SideMetrics();
private:
    std::string name_;
    std::ostream& os;
    size_t n_cmds;
    size_t n_blocks;
};