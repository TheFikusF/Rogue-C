#pragma once
#include "Vec2.h"
#include "System.h"
#include "Timer.h"
#include "Health.h"

struct Player {
    float speed;
    Health health;
    Timer shootCooldown;
    Timer abilityDuration;
    Timer abilityAmplitude;
};

class PlayerSystem : public Core::System {
public:
    PlayerSystem();

    void Update(float dt) override;
};