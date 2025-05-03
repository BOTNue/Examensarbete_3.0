#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float max_hp;
    bool active;
    bool on_ground;
} Player;

Player player_1 = {
    .position = {SCREENWIDTH / 4, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .active = true,
    .on_ground = true,
};

Player player_2 = {
    .position = {SCREENWIDTH * 0.75, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .active = true,
    .on_ground = true,
};

int main()
{
    float gravity = 0.5f;
    float jump_power = -12.0f;
    float ground_y = SCREENHEIGHT - player_1.size.y;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2D fighting game");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        // Player movements
        if (IsKeyDown(KEY_A))
        {
            player_1.position.x -= player_1.velocity.x * GetFrameTime();
        }

        if (IsKeyDown(KEY_D))
        {
            player_1.position.x += player_1.velocity.x * GetFrameTime();
        }

        if (IsKeyDown(KEY_J))
        {
            player_2.position.x -= player_2.velocity.x * GetFrameTime();
        }

        if (IsKeyDown(KEY_L))
        {
            player_2.position.x += player_2.velocity.x * GetFrameTime();
        }

        if (IsKeyPressed(KEY_W) && player_1.on_ground)
        {
            player_1.velocity.y = jump_power;
            player_1.on_ground = false;
        }

        if (IsKeyPressed(KEY_I) && player_2.on_ground)
        {
            player_2.velocity.y = jump_power;
            player_2.on_ground = false;
        }

        player_1.velocity.y += gravity;

        player_1.position.y += player_1.velocity.y;

        player_2.velocity.y += gravity;

        player_2.position.y += player_2.velocity.y;

        if (player_1.position.y >= ground_y)
        {
            player_1.position.y = ground_y;
            player_1.velocity.y = 0;
            player_1.on_ground = true;
        }

        if (player_2.position.y >= ground_y)
        {
            player_2.position.y = ground_y;
            player_2.velocity.y = 0;
            player_2.on_ground = true;
        }

        if (player_1.position.x < 0)
        {
            player_1.position.x = 0;
        }

        if (player_1.position.x + player_1.size.x >= SCREENWIDTH)
        {
            player_1.position.x = SCREENWIDTH - player_1.size.x;
        }

        if (player_2.position.x < 0)
        {
            player_2.position.x = 0;
        }

        if (player_2.position.x + player_2.size.x >= SCREENWIDTH)
        {
            player_2.position.x = SCREENWIDTH - player_2.size.x;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleV(player_1.position, player_1.size, GREEN);
        DrawRectangleV(player_2.position, player_2.size, RED);
        EndDrawing();
    }
    CloseWindow();
}