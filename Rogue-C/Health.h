#pragma once
#include "core.h"
#include <functional>

struct Health {
    int current;
    int max;
    bool invincible;
    Timer invincibilityTime;
    std::function<void (void)> onDeath;
    std::function<void (int)> onTakeDamage;

    Health() = default;
    Health(int max);
    Health(int max, float invincibilityTime, std::function<void (void)> onDeath);

    void TakeDamage(int amount);
    void Heal(int amount);
    void Process(float dt);
    int GetCurrent();
    int GetMax();
};