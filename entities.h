#pragma once

#include "raylib_common_ext.h"
#include "raylib.h"
#include "game_math.h"
#include "ui_elements.h"
#include "timer.h"
#include <iostream>
#include <unordered_map>
#include <functional>

struct Ship
{
    // Draw data
    std::unordered_map<CompassDirection, Texture2D *> textures;
    Texture2D *target_texture = nullptr;
    float scaleFactor = .3f;
    SliderBar* targeted_healthbar;

    // Position data
    Vector2 position = {0};
    float rotation = 0.f;

    // Movement data
    bool isMoving = false;
    Vector2 direction = {0};
    Vector2 target_location = {0};
    float speed = 100.f;

    // Combat data
    float MaxHealth = 100.f;
    float CurrentHealth = MaxHealth;
    Ship *target = nullptr;
    bool targeted = false;
    float damage = 10.f;
    bool isAttacking = false;
    float attackRate = .5f;
    float attackTimer = attackRate;
    bool canDealDamage = true;

    // Methods

    Ship()
    {
        targeted_healthbar = new SliderBar();
        targeted_healthbar->value = CurrentHealth / MaxHealth;
        targeted_healthbar->frontColor = GREEN;
        targeted_healthbar->backColor = GRAY;
        targeted_healthbar->size = {100, 5};
        targeted_healthbar->SetPositionWithNormalizedValues({static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, position.x, position.y);
    }

    void Update(const float &frameTime)
    {
        if (isMoving)
        {
            position = position + (direction * speed * frameTime);
            if (Magnitude(target_location - position) < 50)
            {
                isMoving = false;
            }
        }

        if(isAttacking && canDealDamage)
        {
            DealDamage(target, damage);
        }

        if(!canDealDamage)
        {
            attackTimer -= frameTime;
            if(attackTimer <= 0.f)
            {
                canDealDamage = true;
                attackTimer = attackRate;
            }
        }
    }

    void Draw(float frameTime)
    {
        if (targeted && target_texture)
        {
            // Draw target circle
            ext::DrawTexture2D(*target_texture, position, .06f, Pivot::CENTER, WHITE);
            // Draw health bar
            targeted_healthbar->position = {position.x - (targeted_healthbar->size.x / 2), position.y - (textures[CompassDirection::NORTH]->height * scaleFactor / 2) - 20.f};
            targeted_healthbar->Draw(frameTime);
        }
        if(!isAttacking)
            ext::DrawTexture2D(*textures[getCompassDirection(direction)], position, scaleFactor, Pivot::CENTER, WHITE);
        else
        {
            ext::DrawTexture2D(*textures[getCompassDirection(Normalize(target->position - position))], position, scaleFactor, Pivot::CENTER, WHITE);

        }
    }

    void EnableAttackCallback()
    {
        canDealDamage = true;
    }

    void Attack()
    {
        if(isAttacking)
            StopAttack();
        else
            StartAttack();
    }

    void StartAttack()
    {
        if(target && target != this)
        {
            std::cout << "Attacking target!" << std::endl;
            isAttacking = true;
        }
    }

    void StopAttack()
    {
        std::cout << "Stopping attack on target!" << std::endl;
        isAttacking = false;
    }

    void SetTarget(Ship* t)
    {
        target = t;
        StopAttack();
    }

    float ReceiveDamage(float amount)
    {
        CurrentHealth -= amount;
        if(CurrentHealth < 0.f)
            CurrentHealth = 0.f;
        targeted_healthbar->value = CurrentHealth / MaxHealth;
        return amount;
    }

    float DealDamage(Ship* target, float amount)
    {
        canDealDamage = false;
        return target->ReceiveDamage(amount);
    }
};

struct Map
{
    Vector2 size = {100000, 100000};
    Vector2 GetCenter() { return (size / 2); };
    Texture2D *background;
    Texture2D *foreground;

    // Methods
    void Draw()
    {
        ext::DrawTexture2D(*background, GetCenter(), 1, Pivot::CENTER, WHITE);
    }
};

struct LaserShot
{
    Texture2D* texture;
    double speed = .25;
    Vector2 target = {0};
    double t = 0.0;
    double startTime;
    Vector2 position;
    Vector2 startPosition;
    bool done = false;

    LaserShot(Ship *target, Vector2 startPos)
        : target(target->position), startTime(GetTime()), position(startPos), startPosition(startPos)
    {
        texture = new Texture2D(LoadTexture("./img/laser_blue.png"));
        t = 0;
    }


    void Draw(const float frameTime)
    {
        //if(done) return;
        t = (GetTime() - startTime) / speed;

        position = Lerp(startPosition, target, t);

        if(t >= 1.0)
        {
            done = true;
            startTime = GetTime();
        }

        ext::DrawTexture2D(*texture, position, .55, Pivot::CENTER, WHITE);
    }
};
