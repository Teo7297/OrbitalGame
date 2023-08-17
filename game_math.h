#pragma once

#include "raylib.h"
#include <cmath>
#include <algorithm>

Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
Vector2 operator-(const Vector2 &lhs, const float &f)
{
    return {lhs.x - f, lhs.y - f};
}
Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

Vector2 operator*(const Vector2 &vec, const float &c)
{
    return {vec.x * c, vec.y * c};
}
Vector2 operator/(const Vector2 &vec, const float &c)
{
    return {vec.x / c, vec.y / c};
}

Vector2 Lerp(const Vector2 &start, const Vector2 &end, float t)
{
    t = std::max(0.0f, std::min(1.0f, t)); // Ensure t stays between 0 and 1
    float x = start.x + (end.x - start.x) * t;
    float y = start.y + (end.y - start.y) * t;
    return {x, y};
}

float Magnitude(const Vector2 &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}
Vector2 Normalize(const Vector2 &v)
{
    float mag = Magnitude(v);
    if (mag != 0.0f)
    {
        float invMag = 1.0f / mag;
        return {v.x * invMag, v.y * invMag};
    }
    else
        return {0.f, 0.f};
}

// ############ COMPASS MATH ######################

/// @brief Compass directions used to load textures based on movement
enum class CompassDirection
{
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
};

/// @brief Compute the correct compass direction based on movement direction
/// @param direction normalized direction vector
/// @return CompassDirection enum value
CompassDirection getCompassDirection(Vector2 direction)
{
    const double x = direction.x;
    const double y = direction.y;
    const double sqrt2 = 1.41421356237; // Square root of 2

    // Determine the octant by checking the relative magnitudes of x and y
    if (y > 0.2)
    {
        if (x > 0.2)
        {
            if (x > y * sqrt2)
            {
                return CompassDirection::EAST;
            }
            else
            {
                return CompassDirection::SOUTHEAST;
            }
        }
        else if (x < -0.2)
        {
            if (-x > y * sqrt2)
            {
                return CompassDirection::WEST;
            }
            else
            {
                return CompassDirection::SOUTHWEST;
            }
        }
        else
        {
            return CompassDirection::SOUTH;
        }
    }
    else if (y < -0.2)
    {
        if (x > 0.2)
        {
            if (x > -y * sqrt2)
            {
                return CompassDirection::EAST;
            }
            else
            {
                return CompassDirection::NORTHEAST;
            }
        }
        else if (x < -0.2)
        {
            if (-x > -y * sqrt2)
            {
                return CompassDirection::WEST;
            }
            else
            {
                return CompassDirection::NORTHWEST;
            }
        }
        else
        {
            return CompassDirection::NORTH;
        }
    }
    else
    {
        if (x > 0.2)
        {
            return CompassDirection::EAST;
        }
        else if (x < -0.2)
        {
            return CompassDirection::WEST;
        }
        else
        {
            // Vector is at the origin; direction can't be determined
            return CompassDirection::NORTH;
        }
    }
}
// ############ END COMPASS MATH ######################
