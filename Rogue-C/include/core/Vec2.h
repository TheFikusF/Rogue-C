#pragma once
#include "./include/core/MathF.h"
#include "./include/core/serialization/Serializable.h"
#include <format>

struct Vec2 : public Serializable {
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

    void Read(std::string name, void* value) override {
        if(name.compare("x") == 0) x = *(float*)value;
        if(name.compare("y") == 0) y = *(float*)value;
    }

    std::string Write() override {
        return std::format("x:{}\ny:{}", x, y).c_str();
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
        return Vec2(MathF::Lerp(a.x, b.x, t), MathF::Lerp(a.y, b.y, t));
    }

    static constexpr Vec2 InvLerp(const Vec2 a, const Vec2 b, const float value) {
        return Vec2(MathF::InvLerp(a.x, b.x, value), MathF::InvLerp(a.y, b.y, value));
    }

    static constexpr Vec2 Remap(const Vec2 fromMin, const Vec2 fromMax, const Vec2 toMin, const Vec2 toMax, const float value) {
        return Vec2(MathF::Remap(fromMin.x, fromMax.x, toMin.x, toMax.x, value), MathF::Remap(fromMin.y, fromMin.y, toMin.y, toMax.y, value));
    }

    //FIXME: NOT WORKING, NEED TO BE REMADE LATER
    static constexpr Vec2 SmoothDamp(const Vec2 current, const Vec2 target, Vec2& currentVelocity, const float smoothTime, 
        const float deltaTime, float maxSpeed = INFINITY) {

        return Vec2(MathF::SmoothDamp(current.x, target.x, currentVelocity.x, smoothTime, deltaTime, maxSpeed), 
            MathF::SmoothDamp(current.y, target.y, currentVelocity.x, smoothTime, deltaTime, maxSpeed));
    }

    static constexpr Vec2 MoveTowards(const Vec2 current, const Vec2 target, float maxDelta) {
        return Vec2(MathF::MoveTowards(current.x, target.x, maxDelta), MathF::MoveTowards(current.y, target.y, maxDelta));
    }
    
    static constexpr Vec2 CubicBezier(const Vec2 p1, const Vec2 p2, const float t) {
        float u = 1 - t;
        float t2 = t * t;
        float u2 = u * u;
        float u3 = u2 * u;
        float t3 = t2 * t;

        return (p1 * 3 * u2 * t) + (p2 * 3 * u * t2) + (Vec2(1, 1) * t3);
    }
};