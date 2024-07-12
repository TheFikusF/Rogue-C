#include "Health.h"
#include "math.h"
#include <iostream>

void Health::TakeDamage(int amount) {
    current = std::max(0, current - amount);
    
    if(current == 0) {
        onDeath();
        std::cout << "DAMAGED" << std::endl;
    }
}

int Health::GetCurrent() { return current; }

int Health::GetMax() { return max; }
