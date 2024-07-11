#pragma once
#include <vector>
#include <functional>

class Timer {
private:
    bool started;
    bool finished;

    float time;
    float currentTime;

public:

    Timer() = default;
    Timer(float time);

    void Process(float ds);
    void Start();
    bool GetFinished();
    void Stop();
};
