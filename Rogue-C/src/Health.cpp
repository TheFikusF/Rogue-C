#include "Health.h"
#include "math.h"

Health::Health(int max) : max(max), current(max), onDeath([]() -> void {}), invincibilityTime(0.2f), invincible(false) {}

Health::Health(int max, float invincibilityTime, std::function<void(void)> onDeath) : max(max), current(max), onDeath(onDeath), invincibilityTime(invincibilityTime), invincible(false) {}

void Health::TakeDamage(int amount) {
    if(invincible == true) {
        return;
    }

    current = std::max(0, current - amount);
    invincible = true;
    invincibilityTime.Start();

    if(current == 0) {
        onDeath();
    }
}

void Health::Heal(int amount) {
    if(current == 0) {
        return;
    }

    current = std::max(0, current + amount);
}

int Health::GetCurrent() { return current; }

int Health::GetMax() { return max; }

void Health::Process(float dt) {
    if(invincibilityTime.Check(dt)) {
        invincible = false;
    }
}
