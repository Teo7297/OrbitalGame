#pragma once

#include "raylib.h"

enum class Pivot
{
    TOPLEFT,
    CENTER
};
namespace ext
{

    void DrawTexture2D(Texture2D texture, Vector2 position, float scale, Pivot pivot, Color tint)
    {
        auto scaled_w = (float)texture.width * scale;
        auto scaled_h = (float)texture.height * scale;

        float pivot_x = 0.f;
        float pivot_y = 0.f;

        switch (pivot)
        {
        case Pivot::TOPLEFT:
            break;
        case Pivot::CENTER:
            pivot_x = scaled_w / 2;
            pivot_y = scaled_h / 2;
        default:
            break;
        }

        Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        Rectangle dest = {position.x - pivot_x, position.y - pivot_y, scaled_w, scaled_h};
        Vector2 origin = {0.0f, 0.0f};

        DrawTexturePro(texture, source, dest, origin, 0.f, tint);
    }

}