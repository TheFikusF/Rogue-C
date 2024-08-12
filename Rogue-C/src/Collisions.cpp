#include "Collisions.h"

bool PointInRectangle(Vec2 point, Vec2 position, Vec2 scale) { 
    return point.x >= position.x - scale.x &&
            point.x <= position.x + scale.x &&
            point.y >= position.y - scale.y &&
            point.y <= position.y + scale.y;
}