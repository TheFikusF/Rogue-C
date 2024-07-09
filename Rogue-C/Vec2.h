#pragma once
#include <math.h>

class Vec2
{
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    inline const Vec2 operator+ (const Vec2 &other) const;
    inline const Vec2 operator- (const Vec2 &other) const;
    inline const Vec2 operator* (const float &other) const;
    inline const Vec2 operator/ (const float &other) const;
    inline const bool operator== (const Vec2 &other) const;
    
    inline const Vec2& operator+= (const Vec2 &other);
    inline const Vec2& operator-= (const Vec2 &other);
    inline const Vec2& operator*= (const float &other);
    inline const Vec2& operator/= (const float &other);
    
    inline const float GetMagnitude() const;
    inline const Vec2 GetNormalized() const;

    static const float Distance(const Vec2 &a, const Vec2 &b);
};

Vec2::Vec2() : x(0), y(0) { }
Vec2::Vec2(float x, float y) : x(x), y(y) { }

inline const Vec2 Vec2::operator+(const Vec2 &other) const
{
    return Vec2(x + other.x, y + other.y);
}

inline const Vec2 Vec2::operator-(const Vec2 &other) const
{
    return Vec2(x - other.x, y - other.y);
}

inline const Vec2 Vec2::operator*(const float &other) const
{
    return Vec2(x * other, y * other);
}

inline const Vec2 Vec2::operator/(const float &other) const
{
    return Vec2(x / other, y / other);
}

inline const bool Vec2::operator==(const Vec2 &other) const
{
    return x == other.x && y == other.y;
}

inline const Vec2 &Vec2::operator+=(const Vec2 &other)
{
    x = x + other.x;
    y = y + other.y;
    return *this;
}

inline const Vec2 &Vec2::operator-=(const Vec2 &other)
{
    x = x - other.x;
    y = y - other.y;
    return *this;
}

inline const Vec2 &Vec2::operator*=(const float &other)
{
    x = x * other;
    y = y * other;
    return *this;
}

inline const Vec2 &Vec2::operator/=(const float &other)
{
    x = x / other;
    y = y / other;
    return *this;
}

inline const float Vec2::GetMagnitude() const
{
    return sqrt(x * x + y * y);
}

inline const Vec2 Vec2::GetNormalized() const
{
    float magnitude = GetMagnitude();
    if(magnitude == 0)
    {
        return Vec2();
    }

    return (*this) / magnitude;
}

inline const float Vec2::Distance(const Vec2 &a, const Vec2 &b)
{
    return (a - b).GetMagnitude();
}
