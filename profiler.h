//
// Created by giovanni on 07.03.16.
//
#ifndef EX2_PROFILER_H
#define EX2_PROFILER_H
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>

class Profiler{
public:
    Profiler(std::string n0="");
    inline ~Profiler();
private:
    std::string name;
    timespec t0,t1;
    float elapsed();
};

Profiler::Profiler(std::string n0):name(n0){
        clock_gettime(CLOCK_MONOTONIC, &t0);
}

inline Profiler::~Profiler() {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    std::cout<<"function "<<name<<"\t took "<<elapsed()<< "\t ns to complete"<<std::endl;
}

float Profiler::elapsed() {
    return (t1.tv_nsec-t0.tv_nsec)*1e-9+(t1.tv_sec-t0.tv_sec);
}

#endif //EX2_PROFILER_H
