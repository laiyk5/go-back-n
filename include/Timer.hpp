#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdio>
#include <sys/time.h>
#include <cstddef>

// timer with fix timeout interval TIMEOUT
class Timer{
private:
    double time;
public:
    Timer() 
    {; }

    void start(){
        time = get_time();
    }
    void stop(){
        time = 0;
    }
    bool timeout(double tol)
    {
        double interval = get_time() - time;
        if(time != 0 && interval > tol){
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