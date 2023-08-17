// Press F5 to Debug

#include "raylib_common_ext.h"
#include "game_math.h"
#include "entities.h"
#include "ui_elements.h"
#include "timer.h"
#include <iostream>
#include <string>
#include <vector>
// #include <functional>

const int WIDTH = 960;
const int HEIGHT = 540;
const Vector2 SCREEN_CENTER = {WIDTH / 2, HEIGHT / 2};
const Vector2 SCREEN_SIZE = {WIDTH, HEIGHT};

std::vector<Ship *> *entities = new std::vector<Ship *>();
bool canMove = false;

void ProcessInput(Ship &player, Camera2D &camera)
{

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 click_distance = GetMousePosition() - SCREEN_CENTER;
        Vector2 target_point = player.position + click_distance;
        // Check if enemy is clicked

        for (auto e : *entities)
        {
            if (Magnitude(target_point - e->position) < 100.f)
            {
                if (player.target != e)
                {
                    if (player.target != nullptr)
                        player.target->targeted = false;
                    e->targeted = true;
                    player.SetTarget(e);
                    std::cout << "Target acquired! HP: " << e->CurrentHealth << std::endl;
                }
                canMove = false;
                return;
            }
        }
        canMove = true;
    }

    if(canMove && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 click_distance = GetMousePosition() - SCREEN_CENTER;
        Vector2 target_point = player.position + click_distance;

         // Check if movement is valid
        if (Magnitude(click_distance) < 100.f)
            return;

        // Move
        player.target_location = target_point;
        player.direction = Normalize(click_distance);
        player.isMoving = true;
    }

    

    auto wheelMovement = GetMouseWheelMoveV();
    camera.zoom += wheelMovement.x;
    camera.zoom += wheelMovement.y;
    if(camera.zoom < 1.f)
        camera.zoom = 1.f;


    if(IsKeyPressed(KEY_LEFT_CONTROL))
    {
        player.Attack();
    }
}

void test()
{
    std::cout << "hello" << std::endl;
}

int main()
{

    // App Configuration
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Orbital Warfare 3");
    SetTargetFPS(60);

    // Map init
    Map map;
    map.background = new Texture2D(LoadTexture("./img/bg1.png"));

    // Player init
    Ship player;
    player.textures[CompassDirection::NORTH]     = new Texture2D(LoadTexture("./img/ship/north.png"));
    player.textures[CompassDirection::NORTHEAST] = new Texture2D(LoadTexture("./img/ship/northeast.png"));
    player.textures[CompassDirection::EAST]      = new Texture2D(LoadTexture("./img/ship/east.png"));
    player.textures[CompassDirection::SOUTHEAST] = new Texture2D(LoadTexture("./img/ship/southeast.png"));
    player.textures[CompassDirection::SOUTH]     = new Texture2D(LoadTexture("./img/ship/south.png"));
    player.textures[CompassDirection::SOUTHWEST] = new Texture2D(LoadTexture("./img/ship/southwest.png"));
    player.textures[CompassDirection::WEST]      = new Texture2D(LoadTexture("./img/ship/west.png"));
    player.textures[CompassDirection::NORTHWEST] = new Texture2D(LoadTexture("./img/ship/northwest.png"));
    // player.target_texture = new Texture2D(LoadTexture("./img/ship/target.png"));
    player.speed    = 500.f;
    player.position = map.GetCenter();
    entities->push_back(&player);

    // Player UI
    SliderBar healthbar            = SliderBar();
              healthbar.value      = .75f;
              healthbar.frontColor = GREEN;
              healthbar.backColor  = GRAY;
              healthbar.size       = {150, 25};
    healthbar.SetPositionWithNormalizedValues(SCREEN_SIZE, 0.03f, 0.94f);

    UI_Text fps_counter;
    fps_counter.SetPositionWithNormalizedValues(SCREEN_SIZE, 0.03, 0.03);

    // Enemy init
    Ship enemy;
    enemy.textures[CompassDirection::NORTH]     = new Texture2D(LoadTexture("./img/ship/north.png"));
    enemy.textures[CompassDirection::NORTHEAST] = new Texture2D(LoadTexture("./img/ship/northeast.png"));
    enemy.textures[CompassDirection::EAST]      = new Texture2D(LoadTexture("./img/ship/east.png"));
    enemy.textures[CompassDirection::SOUTHEAST] = new Texture2D(LoadTexture("./img/ship/southeast.png"));
    enemy.textures[CompassDirection::SOUTH]     = new Texture2D(LoadTexture("./img/ship/south.png"));
    enemy.textures[CompassDirection::SOUTHWEST] = new Texture2D(LoadTexture("./img/ship/southwest.png"));
    enemy.textures[CompassDirection::WEST]      = new Texture2D(LoadTexture("./img/ship/west.png"));
    enemy.textures[CompassDirection::NORTHWEST] = new Texture2D(LoadTexture("./img/ship/northwest.png"));
    enemy.target_texture                        = new Texture2D(LoadTexture("./img/ship/target.png"));
    enemy.speed                                 = 500.f;
    enemy.position                              = map.GetCenter();
    entities->push_back(&enemy);

    // Camera init
    Camera2D camera;
    camera.target   = player.position;
    camera.offset   = SCREEN_CENTER;
    camera.zoom     = 1.0f;
    camera.rotation = 0.f;

    LaserShot *laser = new LaserShot(&enemy, player.position - 200.f);

    while (!WindowShouldClose())
    {
        const float frameTime = GetFrameTime();

        // ############### PROCESS INPUT ###############
        ProcessInput(player, camera);
        player.Update(frameTime);
        enemy.Update(frameTime);

        // ############### BEGIN DRAW ###############
        BeginDrawing();

        // ############### GAME WORLD RENDERING ##################
        BeginMode2D(camera);

        ClearBackground(BLACK);

        // Entities update
        map.Draw();

        enemy.Draw(frameTime);
        player.Draw(frameTime);
        laser->Draw(frameTime);
            // Camera update
            camera.target = player.position;

        EndMode2D();

        // ############### END GAME WORLD RENDERING ##################

        // ############### UI RENDERING ##################
        healthbar.value = player.CurrentHealth / player.MaxHealth;
        healthbar.Draw(frameTime);

        const std::string s = "FPS: " + std::to_string((int)GetFPS());
        fps_counter.message = s;
        fps_counter.Draw(frameTime);

        // ############### END UI RENDERING ##################
        EndDrawing();

        // ############### END DRAW ###############

        // ############### DEBUG STUFF ##################
        // std::cout << (int)getCompassDirection(player.direction) << std::endl;
        player.CurrentHealth = 35.f;

    }

    return 0;
};