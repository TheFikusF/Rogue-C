#pragma once
#include "./include/core/ecs/System.h"
#include "./include/core/Transform.h"
#include "./include/core/Vec2.h"

class InputSystem : public Core::System {
private:
    Vec2 _movementAxis;
    Vec2 _shootingAxis;
    bool _shootPressed;
    Entity _player;

public:
    void Update(float dt) override;
    
    static const Vec2 GetMovementAxis();
    static const Vec2 GetShootingAxis();
    static const bool IsShooting();
    static void SetPlayer(Entity player);
};