#pragma once
#include <math.h>

struct Vec2
{
    float x, y;

    Vec2();
    Vec2(float x, float y);

    const Vec2 operator+ (const Vec2 other) const;
    const Vec2 operator- (const Vec2 other) const;
    const Vec2 operator* (const float other) const;
    const Vec2 operator/ (const float other) const;
    const bool operator== (const Vec2 other) const;
    
    const Vec2& operator+= (const Vec2 other);
    const Vec2& operator-= (const Vec2 other);
    const Vec2& operator*= (const float other);
    const Vec2& operator/= (const float other);
    
    inline const float GetMagnitude() const;
    inline const float GetMagnitudeSquared() const;
    inline const Vec2 GetNormalized() const;

    inline static const float Distance(const Vec2 a, const Vec2 b);
    inline static const float DistanceSquared(const Vec2 a, const Vec2 b);
    inline static const float Dot(const Vec2 a, const Vec2 b);
    inline static const Vec2 Lerp(const Vec2 a, const Vec2 b, const float t);
};