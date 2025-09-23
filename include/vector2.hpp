#pragma once

#include <cmath>
#include <functional>

template <typename T>
class Vector2
{
public:
    T x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(T x_value, T y_value) : x{x_value}, y{y_value} {}

    bool operator==(const Vector2& other) const 
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2 &other) const
    {
        return x != other.x || y != other.y;
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator+=(const Vector2 &other) 
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalize() const
    {
        float len = length();
        return len > 0 ? Vector2(x / (x+y), y / (x+y)) : Vector2(0, 0);
    }
};


namespace std {
    template<>
    struct hash<Vector2<int>> {
        size_t operator()(const Vector2<int>& pos) const noexcept {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };
}
