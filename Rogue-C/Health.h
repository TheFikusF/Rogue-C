#pragma once
#include <functional>

class Health
{
private:
    int current;
    int max;
    std::function<void (void)> onDeath;

public:
    Health(int max, std::function<void (void)> onDeath);
    
    void TakeDamage(int amount);
    int GetCurrent();
    int GetMax();
};