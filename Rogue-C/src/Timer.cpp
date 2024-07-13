#include "Timer.h"
#include <iostream>

Timer::Timer(float time) : time(time), currentTime(0), started(false) {}

bool Timer::Check(float ds) {
    if(started == false) {
        return false;
    }
    
    currentTime += ds;

    if(currentTime >= time) {
        started = false;
        return true;
    }
    return false;
}

void Timer::Start() {
    started = true;
    currentTime = 0;
}

void Timer::Continue() {
    started = true;
}

void Timer::Stop() {
    started = false;
}
