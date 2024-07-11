#pragma once
#include <math.h>

class Vec2
{
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    const Vec2 operator+ (const Vec2 &other) const;
    const Vec2 operator- (const Vec2 &other) const;
    const Vec2 operator* (const float &other) const;
    const Vec2 operator/ (const float &other) const;
    const bool operator== (const Vec2 &other) const;
    
    const Vec2& operator+= (const Vec2 &other);
    const Vec2& operator-= (const Vec2 &other);
    const Vec2& operator*= (const float &other);
    const Vec2& operator/= (const float &other);
    
    const float GetMagnitude() const;
    const Vec2 GetNormalized() const;

    static const float Distance(const Vec2& a, const Vec2& b);
    static const float Dot(const Vec2 &a, const Vec2 &b);
};