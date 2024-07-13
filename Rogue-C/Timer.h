#pragma once
#include <vector>
#include <functional>

class Timer {
private:
    bool started;

    float time;
    float currentTime;

public:

    Timer() = default;
    Timer(float time);

    bool Check(float ds);
    void Start();
    void Continue();
    void Stop();
};
