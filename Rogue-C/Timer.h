#pragma once
#include <vector>
#include <functional>

class Timer {
public:
    bool started;
    bool finished;

    float time;
    float currentTime;

    void Process(float ds);
    void Start();
    bool GetFinished();
    void Stop();
};
