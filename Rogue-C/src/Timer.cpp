#include "./include/core/Timer.h"
#include "./include/raylib/raymath.h"
#include <iostream>

Timer::Timer(float time) : time(time), currentTime(0), started(false) {}

bool Timer::Check(float dt) {
    if(started == false) {
        return false;
    }
    
    currentTime += dt;

    if(currentTime >= time) {
        started = false;
        return true;
    }
    return false;
}

bool Timer::InvCheck(float dt) {
    return Check(dt) == false && started;
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
