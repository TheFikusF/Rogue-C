#pragma once
#include <vector>
#include <functional>

class Timer {
private:
    int id;
    float time;
    float currentTime;
    std::function<void (void)> callback;

    static std::vector<Timer> timers;
    void Process(float ds);

public:
    Timer(float time,  std::function<void (void)> callback);


    static void ProcessAll(float ds);
};
