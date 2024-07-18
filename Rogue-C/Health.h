#pragma once
#include <functional>
#include "LOG.h"
#include "Timer.h"

struct Health {
    int current;
    int max;
    bool invincible;
    Timer invincibilityTime;
    std::function<void (void)> onDeath;

    Health() = default;
    Health(int max);
    Health(int max, float invincibilityTime, std::function<void (void)> onDeath);

    void TakeDamage(int amount);
    void Process(float dt);
    int GetCurrent();
    int GetMax();
};