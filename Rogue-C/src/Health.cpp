#include "Health.h"
#include "math.h"

void Health::TakeDamage(int amount) {
    current = std::max(0, current - amount);
    
    if(current == 0) {
        onDeath();
    }
}

int Health::GetCurrent() { return current; }

int Health::GetMax() { return max; }
