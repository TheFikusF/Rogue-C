#include "Timer.h"
#include <iostream>

void Timer::Process(float ds) {
    if(started == false) {
        return;
    }
    
    currentTime += ds;

    if(currentTime >= time) {
        finished = true;
    }
}

void Timer::Start() {
    started = true;
    finished = false;
    currentTime = 0;
}

bool Timer::GetFinished() {
    if(finished == true) {
        finished == false;
        return true;
    }
    return false;
}

void Timer::Stop() {
    started = false;
    finished = false;
}
