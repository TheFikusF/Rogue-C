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
public:
    PickUpSystem();
    
    static void Spawn(Vec2 position);
    void OnTrigger(const Collision2D& collision) override;
};
