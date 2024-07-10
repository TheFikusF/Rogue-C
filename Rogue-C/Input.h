#pragma once
#include "Vec2.h"

class Input
{
private:
    Vec2 movementAxis;
    Vec2 shootingAxis;
    bool shootPressed;
    
    Input() { };

    static Input _input;
public:
    static void Process(Vec2 center, float ds);
    static const Vec2 GetMovementAxis();
    static const Vec2 GetShootingAxis();
    static const bool IsShooting();
};