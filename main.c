#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

typedef enum
{
    MAINMENU,
    CONTROLS,
    GAME,
    ENDING,
} Game_state;

typedef struct
{
    Rectangle hitbox;
    int damage;
    float duration;
    float timer;
    bool active;
    bool has_hit;
} Attack;

typedef struct
{
    Texture2D texture;
    int frame_count;
    int current_frame;
    int frame_width;
    int frame_height;
    int frame_speed;
    int frame_counter;
} Animation;

typedef struct
{
    Animation * current_animation;
} Animator;

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float hp;
    float max_hp;
    Attack attack;
    bool on_ground;
    bool is_attacking;
    bool active;
    bool facing_right;
} Player;

typedef struct
{
    Vector2 position;
    Vector2 size;
} Stage;

Vector2 player_1_start = {SCREENWIDTH * 0.25, SCREENHEIGHT * 0.8};
Vector2 player_2_start = {SCREENWIDTH * 0.75 - 75, SCREENHEIGHT * 0.8};

Player player_1 = {
    .position = {SCREENWIDTH * 0.25, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .hp = 100.0f,
    .max_hp = 100.0f,
    .on_ground = true,
    .is_attacking = false,
    .active = true,
    .facing_right = true,
};

Player player_2 = {
    .position = {SCREENWIDTH * 0.75 - 75, SCREENHEIGHT * 0.8},
    .velocity = {125.0f, 0.0f},
    .size = {75, 150},
    .hp = 100.0f,
    .max_hp = 100.0f,
    .on_ground = true,
    .is_attacking = false,
    .active = true,
    .facing_right = false,
};

Stage stage = {
    .position = {0, SCREENHEIGHT * 0.8},
    .size = {SCREENWIDTH, SCREENHEIGHT * 0.2},
};

Animation player_1_idle_ani;
Animation player_1_run_ani;
Animation player_1_attack_ani;

Animation player_2_idle_ani;
Animation player_2_run_ani;
Animation player_2_attack_ani;

Animator player_1_animator;

Animator player_2_animator;

Rectangle get_player_bounds(Player *p)
{
    return (Rectangle) {p->position.x, p->position.y, p->size.x, p->size.y};
}

void start_attack(Player *player)
{
    if (!player->is_attacking)
    {
        player->is_attacking = true;
        player->attack.active = true;
        player->attack.has_hit = false;
        player->attack.timer = 0.0f;
        player->attack.duration = 0.4f;
        player->attack.damage = 10;
        if (player->facing_right)
        {   
            player->attack.hitbox = (Rectangle){
            player->position.x + player->size.x + 100, 
            player->position.y + 70,
            20,
            player->size.y * 0.2 
            };
        }
        if (!player->facing_right)
        {
            player->attack.hitbox = (Rectangle){
            player->position.x - 115, 
            player->position.y + 70,
            20,
            player->size.y * 0.2 
            };
        }
        player_1_animator.current_animation = &player_1_attack_ani;
        player_1_animator.current_animation->current_frame = 0;
        player_1_animator.current_animation->frame_counter = 0;

        player_2_animator.current_animation = &player_2_attack_ani;
        player_2_animator.current_animation->current_frame = 0;
        player_2_animator.current_animation->frame_counter = 0;
    }
}

void update_attack(Player *player, float delta_time)
{
    if (player->attack.active)
    {
        player->attack.timer += delta_time;
        if (player->attack.timer >= player->attack.duration)
        {
            player->attack.active = false;
            player->is_attacking = false;
        }
    }
}

bool check_attack_hit(Player *attacker, Player *target)
{
    if (!attacker->attack.active || attacker->attack.has_hit)
    {
        return false;
    }
    
    Rectangle targetbounds = get_player_bounds(target);
    if (CheckCollisionRecs(attacker->attack.hitbox, targetbounds))
    {
        attacker->attack.has_hit = true;
        return true;
    }
    return false;
}

void update_animation(Animation *ani)
{
    ani->frame_counter++;
    if (ani->frame_counter >= (60/ani->frame_speed))
    {
        ani->frame_counter = 0;
        ani->current_frame = (ani->current_frame + 1) % ani->frame_count;
    }
}

void draw_animation(Animation *ani, Vector2 position, bool flip)
{
    Rectangle source = {
        .x = ani->frame_width * ani->current_frame,
        .y = 0,
        .width = flip ? ani->frame_width : -ani->frame_width,
        .height = ani->frame_height,
    };

    Rectangle dest = {
        .x = position.x,
        .y = position.y,
        .width = ani->frame_width * 4,
        .height = ani->frame_height * 4,
    };

    Vector2 origin = {dest.width / 2.5, dest.height / 2.2};
    DrawTexturePro(ani->texture, source, dest, origin, 0, WHITE);
}



void reset_player()
{
    player_1.hp = 100;
    player_2.hp = 100;
    
    player_1.position = player_1_start;
    player_2.position = player_2_start;
}

int main()
{
    float gravity = 0.5f;
    float jump_power = -12.0f;
    int winner;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2D fighting game");
    
    player_1_idle_ani = (Animation){
        .texture = LoadTexture("Sprites/IDLE.png"),
        .frame_count = 10,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 10,
        .frame_counter = 0,
    };

    player_1_run_ani = (Animation){
        .texture = LoadTexture("Sprites/RUN.png"),
        .frame_count = 16,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 16,
        .frame_counter = 0
    };

    player_1_attack_ani = (Animation){
        .texture = LoadTexture("Sprites/ATTACK 1.png"),
        .frame_count = 7,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 15,
        .frame_counter = 0
    };

    player_2_idle_ani = (Animation){
        .texture = LoadTexture("Sprites/IDLE.png"),
        .frame_count = 10,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 10,
        .frame_counter = 0,
    };

    player_2_run_ani = (Animation){
        .texture = LoadTexture("Sprites/RUN.png"),
        .frame_count = 16,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 16,
        .frame_counter = 0
    };

    player_2_attack_ani = (Animation){
        .texture = LoadTexture("Sprites/ATTACK 1.png"),
        .frame_count = 7,
        .current_frame = 0,
        .frame_width = 96,
        .frame_height = 96,
        .frame_speed = 15,
        .frame_counter = 0
    };

    Game_state currentstate = MAINMENU;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentstate)
        {
        case MAINMENU:
            DrawText("2D Fighting Game", SCREENWIDTH * 0.37, SCREENHEIGHT * 0.45, 25, WHITE);
            DrawText("Start Game Press [SPACE]", SCREENWIDTH * 0.33, SCREENHEIGHT * 0.45 + 35, 20, WHITE);
            DrawText("Show Controls Press [CTRL]", SCREENWIDTH * 0.32, SCREENHEIGHT * 0.45 + 70, 20, WHITE);

            if (IsKeyPressed(KEY_SPACE))
            {
                currentstate = GAME;
            }
            
            if (IsKeyPressed(KEY_LEFT_CONTROL))
            {
                currentstate = CONTROLS;
            }
            break;
        
        case CONTROLS:
            DrawText("Player 1", SCREENWIDTH * 0.15, SCREENHEIGHT * 0.10, 25, WHITE);
            DrawText("Player 2", SCREENWIDTH * 0.70, SCREENHEIGHT * 0.10, 25, WHITE);

            // Player 1
            DrawText("Walk Right: [D]", SCREENWIDTH * 0.16, SCREENHEIGHT * 0.20, 15, WHITE);
            DrawText("Walk Left: [A]", SCREENWIDTH * 0.16, SCREENHEIGHT * 0.25, 15, WHITE);
            DrawText("Jump: [W]", SCREENWIDTH * 0.18, SCREENHEIGHT * 0.30, 15, WHITE);
            DrawText("Attack: [X]", SCREENWIDTH * 0.17, SCREENHEIGHT * 0.35, 15, WHITE);


            // Player 2
            DrawText("Walk Right: [L]", SCREENWIDTH * 0.72, SCREENHEIGHT * 0.20, 15, WHITE);
            DrawText("Walk Left: [J]", SCREENWIDTH * 0.72, SCREENHEIGHT * 0.25, 15, WHITE);
            DrawText("Jump: [I]", SCREENWIDTH * 0.74, SCREENHEIGHT * 0.30, 15, WHITE);
            DrawText("Attack: [M]", SCREENWIDTH * 0.73, SCREENHEIGHT * 0.35, 15, WHITE);

            DrawText("Back To Main Menu [ENTER]", SCREENWIDTH * 0.28, SCREENHEIGHT / 2, 25, WHITE);
            DrawText("Start Game [SPACE]", SCREENWIDTH * 0.34, SCREENHEIGHT * 0.60, 25, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                currentstate = MAINMENU;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                currentstate = GAME;
            }

            break;

        case GAME:
            if (IsKeyDown(KEY_A))
            {
                player_1.position.x -= player_1.velocity.x * delta_time;
            }

            if (IsKeyDown(KEY_D))
            {
                player_1.position.x += player_1.velocity.x * delta_time;
            }

            if (IsKeyDown(KEY_J))
            {
                player_2.position.x -= player_2.velocity.x * delta_time;
            }

            if (IsKeyDown(KEY_L))
            {
                player_2.position.x += player_2.velocity.x * delta_time;
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

            if (IsKeyPressed(KEY_X))
            {
                start_attack(&player_1);
                printf("player_1 attacked\n");
            }

            update_attack(&player_1, delta_time);

            if (check_attack_hit(&player_1, &player_2))
            {
                player_2.hp -= player_1.attack.damage;
                printf("hit landed\n");
            }

            if (IsKeyPressed(KEY_M))
            {
                start_attack(&player_2);
            }

            update_attack(&player_2, delta_time);

            if (check_attack_hit(&player_2, &player_1))
            {
                player_1.hp -= player_2.attack.damage;
            }

            if (player_1.is_attacking)
            {
                player_1_animator.current_animation = &player_1_attack_ani;
            }
            else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
            {
                player_1_animator.current_animation = &player_1_run_ani;
            }
            else
            {
                player_1_animator.current_animation = &player_1_idle_ani;
            }

            if (player_2.is_attacking)
            {
                player_2_animator.current_animation = &player_2_attack_ani;
            }
            else if (IsKeyDown(KEY_J) || IsKeyDown(KEY_L))
            {
                player_2_animator.current_animation = &player_2_run_ani;
            }
            else
            {
                player_2_animator.current_animation = &player_2_idle_ani;
            }

            update_animation(player_1_animator.current_animation);
            update_animation(player_2_animator.current_animation);

            draw_animation(player_1_animator.current_animation, player_1.position, player_1.facing_right);
            draw_animation(player_2_animator.current_animation, player_2.position, player_2.facing_right);

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

            if (player_1.hp <= 0 || player_2.hp <= 0)
            {
                currentstate = ENDING;
            }
            
            break;

        case ENDING:

            if (player_1.hp <= 0)
            {
                DrawText("Player 2 Wins", SCREENWIDTH * 0.37, SCREENHEIGHT * 0.30, 30, WHITE);
            }
            else
            {
                DrawText("Player 1 Wins", SCREENWIDTH * 0.37, SCREENHEIGHT * 0.30, 30, WHITE);
            }

            DrawText("Return To Main Menu [ENTER]", SCREENWIDTH * 0.22, SCREENHEIGHT * 0.50, 30, WHITE);
            DrawText("Play Again [SPACE]", SCREENWIDTH * 0.33, SCREENHEIGHT * 0.60, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                currentstate = MAINMENU;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                currentstate = GAME;
                reset_player(); 
            }
        }
        EndDrawing();
    }
    CloseWindow();
}