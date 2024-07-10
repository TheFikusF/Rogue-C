#include "Timer.h"
#include <iostream>

std::vector<Timer> Timer::timers;

Timer::Timer(float time, std::function<void (void)> callback) : 
    callback(callback), time(time), id(timers.size()), currentTime(0) {
    timers.push_back(*this);
    //std::cout << "created timer #" << id << " for " << time << " sec" << std::endl;
}

void Timer::ProcessAll(float ds) {
    for(int i = timers.size() - 1; i >= 0; i--) {
        timers[i].Process(ds);
    }
}

void Timer::Process(float ds) {
    currentTime += ds;

    if(currentTime >= time) {
        callback();
        timers.erase(timers.begin() + id);
    }
}
