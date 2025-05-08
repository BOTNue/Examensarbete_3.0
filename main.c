#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

typedef enum
{
    STARTMENU,
    GAME,
} Game_state;

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float hp;
    float max_hp;
    bool active;
    bool on_ground;
} Player;

typedef struct
{
    Vector2 position;
    Vector2 size;
} Stage;

Player player_1 = {
    .position = {SCREENWIDTH * 0.25, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .hp = 100,
    .max_hp = 100,
    .active = true,
    .on_ground = true,
};

Player player_2 = {
    .position = {SCREENWIDTH * 0.75 - 75, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .hp = 100,
    .max_hp = 100,
    .active = true,
    .on_ground = true,
};

Stage stage = {
    .position = {0, SCREENHEIGHT * 0.8},
    .size = {SCREENWIDTH, SCREENHEIGHT * 0.2},
};

int main()
{
    float gravity = 0.5f;
    float jump_power = -12.0f;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2D fighting game");
    SetTargetFPS(60);

    Game_state currentstate = STARTMENU;

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentstate)
        {
        case STARTMENU:
            DrawText("2D Fighting Game", SCREENWIDTH / 2, SCREENHEIGHT / 2, 15, WHITE);
            DrawText("Start game press [SPACE]", SCREENWIDTH / 2, SCREENHEIGHT / 2 + 25, 10, WHITE);

            if (IsKeyPressed(KEY_SPACE))
            {
                currentstate = GAME;
            }
            break;

        case GAME:
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

            if (player_1.position.y + player_1.size.y >= stage.position.y)
            {
                player_1.position.y = stage.position.y - player_1.size.y;
                player_1.velocity.y = 0;
                player_1.on_ground = true;
            }

            if (player_2.position.y + player_2.size.y >= stage.position.y)
            {
                player_2.position.y = stage.position.y - player_2.size.y;
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

            bool is_colliding = 
            player_1.position.x < player_2.position.x + player_2.size.x &&
            player_1.position.x + player_1.size.x > player_2.position.x;

            if (is_colliding)
            {
                float overlap = 0.0f;

                if (player_1.position.x < player_2.position.x)
                {
                    overlap = (player_1.position.x + player_1.size.x) - player_2.position.x;
                    player_1.position.x -= overlap / 2;
                    player_2.position.x += overlap / 2;
                }
                else
                {
                    overlap = (player_2.position.x + player_2.size.x) - player_1.position.x;
                    player_1.position.x += overlap / 2;
                    player_2.position.x -= overlap / 2;
                }
            }

            DrawRectangleV(player_1.position, player_1.size, GREEN);
            DrawRectangleV(player_2.position, player_2.size, RED);
            DrawRectangleV(stage.position, stage.size, WHITE);

            static const int hp_bar_rec_width = 300;
            Rectangle hp_bar_1 = {
                .x = 8,
                .y = 8,
                .width = (player_1.hp / player_1.max_hp) * hp_bar_rec_width,
                .height = 32,
            };

            Rectangle hp_bar_2 = {
                .x = 492,
                .y = 8,
                .width = (player_2.hp / player_2.max_hp) * hp_bar_rec_width,
                .height = 32,
            };
            DrawRectangle(8, 8, 300, 32, DARKGRAY);
            DrawRectangle(492, 8, 300, 32, DARKGRAY);
            DrawRectangleRec(hp_bar_1, RED);
            DrawRectangleRec(hp_bar_2, RED);
        }
        EndDrawing();
    }
    CloseWindow();
}