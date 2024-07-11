#pragma once
#include <functional>

struct Health {
    int current;
    int max;
    std::function<void (void)> onDeath;

    void TakeDamage(int amount);
    int GetCurrent();
    int GetMax();
};