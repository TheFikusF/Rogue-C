#pragma once
#include <math.h>

struct Vec2 {
    float x, y;

    constexpr Vec2() : x(0), y(0) {}
    constexpr Vec2(float x, float y) : x(x), y(y) {}

    constexpr Vec2 operator+(const Vec2 other) const {
        return Vec2(x + other.x, y + other.y);
    }
    constexpr Vec2 operator-(const Vec2 other) const {
        return Vec2(x - other.x, y - other.y);
    }
    constexpr Vec2 operator*(const float other) const {
        return Vec2(x * other, y * other);
    }
    constexpr Vec2 operator/(const float other) const {
        return Vec2(x / other, y / other);
    }
    constexpr bool operator==(const Vec2 other) const {
        return x == other.x && y == other.y;
    }

    constexpr Vec2& operator+=(const Vec2 other) {
        x = x + other.x;
        y = y + other.y;
        return *this;
    }
    constexpr Vec2& operator-=(const Vec2 other) {
        x = x - other.x;
        y = y - other.y;
        return *this;
    }
    constexpr Vec2& operator*=(const float other) {
        x = x * other;
        y = y * other;
        return *this;
    }
    constexpr Vec2& operator/=(const float other) {
        x = x / other;
        y = y / other;
        return *this;
    }

    constexpr float GetMagnitude() const { 
        return sqrt(GetMagnitudeSquared()); 
    }

    constexpr float GetMagnitudeSquared() const { 
        return x * x + y * y; 
    }

    constexpr Vec2 GetNormalized() const {
        float magnitude = GetMagnitude();
        if (magnitude == 0) {
            return Vec2();
        }

        return (*this) / magnitude;
    }

    static constexpr float Distance(const Vec2 a, const Vec2 b) {
        return (a - b).GetMagnitude();
    }

    static constexpr float DistanceSquared(const Vec2 a, const Vec2 b) {
        return (a - b).GetMagnitudeSquared();
    }

    static constexpr float Dot(const Vec2 a, const Vec2 b) {
        return a.x * a.y + b.x * b.y;
    }

    static constexpr Vec2 Lerp(const Vec2 a, const Vec2 b, const float t) {
        return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
    }
};