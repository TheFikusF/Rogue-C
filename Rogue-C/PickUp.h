#pragma once
#include "ECS.h"
#include "System.h"
#include "Player.h"

enum class PickUpType : std::uint8_t {
    Heal,
    DamageBust,
    SpinningSphere,
};

struct PickUp {
    PickUpType type;
};

class PickUpSystem : public Core::System {
public:
    PickUpSystem();
    
    static void Spawn(Vec2 position);
    void OnTrigger(const Collision2D& collision) override;
};
