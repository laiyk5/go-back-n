#ifndef TIMER_HPP
#define TIMER_HPP

#include "gbn.hpp"
// #include <cstdio>
// #include <sys/time.h>
// #include <cstddef>

// timer with fix timeout interval TIMEOUT
class Timer{
private:
    double time;
    // double timeout;
public:
    Timer() 
    // : timeout(0)
    {; }

    void start(){
        time = get_time();
    }
    void stop(){
        time = 0;
    }
    bool istimeout()
    {
        double interval = get_time() - time;
        if(time != 0 && interval > TIMEOUT){
            printf("timeout! %f (s) \n", interval);
            return true;
        }
        return false;
    }
private:
    double get_time(){
        struct timeval t;
        gettimeofday(&t, NULL);
        return t.tv_sec + t.tv_usec/1000000.0;
    }
};

#endif