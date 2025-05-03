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
}Player;

Player player_1 = {
    .position = {SCREENWIDTH / 4, SCREENHEIGHT * 0.8},
    .velocity = {200.0f, 200.0f},
    .size = {75, 200},
    .active = true,
};

Player player_2 = {
    .position = {SCREENWIDTH * 0.75, SCREENHEIGHT * 0.8},
    .velocity = {200.0f, 200.0f},
    .size = {75, 200},
    .active = true,
};

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2d fighting game");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleV(player_1.position, player_1.size, GREEN);
        DrawRectangleV(player_2.position, player_2.size, RED);
        EndDrawing();
    }
    CloseWindow();
}