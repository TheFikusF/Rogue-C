#pragma once
#include <vector>
#include <functional>

class Timer {
private:
    bool started;

    float time;
    float currentTime;

public:

    Timer() : started(false) { }
    Timer(float time);

    bool Check(float dt);
    bool InvCheck(float dt);
    
    void Start();
    void Continue();
    void Stop();

    const bool& IsStarted() const;
    float GetProgress() const;
};
