#include "Health.h"
#include "math.h"

void Health::TakeDamage(int amount) {
    current = std::max(0, current - amount);
    
    LOG("took damage");
    if(current == 0) {
        onDeath();
        LOG("died");
    }
}

int Health::GetCurrent() { return current; }

int Health::GetMax() { return max; }
