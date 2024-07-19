#include "Timer.h"
#include <iostream>
#include "./include/raylib/raymath.h"

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
    if (time <= 0) {
        return;
    }

    started = true;
    currentTime = 0;
}

void Timer::Continue() {
    started = true;
}

void Timer::Stop() {
    started = false;
}

const bool& Timer::IsStarted() const {
    return started;
}

float Timer::GetProgress() const {
    return currentTime / time;
}