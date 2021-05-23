#include "gbn.hpp"
// #include <unistd.h>

void TestTimer(){
    Timer timer;
    timer.start();
    for(int i = 0; i < 1000; i++){
        sleep(0.001);
        if(timer.istimeout()){
        timer.stop();
        }
    }
}


int main(){
    int a;
    int b;
    memcpy(&a, &b, 0);
}