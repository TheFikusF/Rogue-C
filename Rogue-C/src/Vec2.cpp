#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) { }
Vec2::Vec2(float x, float y) : x(x), y(y) { }

const Vec2 Vec2::operator+(const Vec2 &other) const {
    return Vec2(x + other.x, y + other.y);
}

const Vec2 Vec2::operator-(const Vec2 &other) const {
    return Vec2(x - other.x, y - other.y);
}

const Vec2 Vec2::operator*(const float &other) const {
    return Vec2(x * other, y * other);
}

const Vec2 Vec2::operator/(const float &other) const {
    return Vec2(x / other, y / other);
}

const bool Vec2::operator==(const Vec2 &other) const {
    return x == other.x && y == other.y;
}

const Vec2 &Vec2::operator+=(const Vec2 &other) {
    x = x + other.x;
    y = y + other.y;
    return *this;
}

const Vec2 &Vec2::operator-=(const Vec2 &other) {
    x = x - other.x;
    y = y - other.y;
    return *this;
}

const Vec2 &Vec2::operator*=(const float &other) {
    x = x * other;
    y = y * other;
    return *this;
}

const Vec2 &Vec2::operator/=(const float &other) {
    x = x / other;
    y = y / other;
    return *this;
}

const float Vec2::GetMagnitude() const {
    return sqrt(x * x + y * y);
}

const Vec2 Vec2::GetNormalized() const {
    float magnitude = GetMagnitude();
    if(magnitude == 0) {
        return Vec2();
    }

    return (*this) / magnitude;
}

const float Vec2::Distance(const Vec2 &a, const Vec2 &b) {
    return (a - b).GetMagnitude();
}

const float Vec2::Dot(const Vec2& a, const Vec2& b) {
    return a.x * a.y + b.x * b.y;
}

const Vec2 Vec2::Lerp(const Vec2& a, const Vec2& b, const float& t) {
    return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}
