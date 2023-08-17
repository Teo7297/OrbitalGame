#pragma once

#include "raylib.h"
#include <string>

struct UI_Element
{
    Vector2 position = {0};

    /// @brief Set screen position with normalized values
    /// @param screen_size screen size
    /// @param x width  between 0 - 1
    /// @param y height between 0 - 1
    void SetPositionWithNormalizedValues(const Vector2& screen_size, const float& x, const float& y)
    {
        position = {x * screen_size.x, y * screen_size.y};
    };
};

struct SliderBar : UI_Element
{
    float value;
    Color frontColor;
    Color backColor;
    Vector2 size = {0};

    void Draw(const float &frameTime)
    {
        DrawRectangle(position.x, position.y, size.x, size.y, backColor);
        DrawRectangle(position.x, position.y, size.x * value, size.y, frontColor);
    }
};

struct UI_Text : UI_Element
{
    std::string message;
    int fontSize = 18;
    Color color = GREEN;

    void Draw(const float &frameTime)
    {
        DrawText(message.c_str(), position.x, position.y, fontSize, color);
    }
};