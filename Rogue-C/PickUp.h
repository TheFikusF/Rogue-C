#pragma once
#include "ECS.h"
#include "System.h"
#include "Player.h"

enum PickUpType {
    Heal,
    DamageBust,
};

struct PickUp {
    PickUpType type;
};

class PickUpSystem : public System {
    void OnTrigger(const Collision2D& collision) override;
};
