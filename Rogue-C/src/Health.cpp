#include "Health.h"
#include "math.h"

Health::Health(int max, std::function<void(void)> onDeath)
    : current(max), max(max), onDeath(onDeath) {

}

void Health::TakeDamage(int amount) {
    current = std::max(0, current - amount);
    
    if(current == 0) {
        onDeath();
    }
}

int Health::GetCurrent() { return current; }

int Health::GetMax() { return max; }
