#include "player.h"
#include <stdbool.h>

static Player player;  // Variável global interna ao módulo player.c

static int frame = 0;
static float frameTime = 0.1f;
static float frameTimer = 0.0f;
float scale = 4.0f;  // Aumenta 4x (de 16x16 para 64x64 pixels)


void init_player() {
    player.position = (Vector2){ 100, 100 };
    player.sprite = LoadTexture("assets/branquinho-fofinho/idle.png");
    player.vida = 3;

    player.maxFrames = 3; 
}

void update_player() {

    frameTimer += GetFrameTime();
    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        player.frame = (player.frame + 1) % player.maxFrames;  // Loop de 0 a 2
    }

    if(IsKeyDown(KEY_RIGHT)){
        player.position.x += 2;
        player.direction = 1;
    }
    if (IsKeyDown(KEY_LEFT)){
        player.position.x -= 2;
        player.direction = -1;
    }
}

void draw_player() {
    float scale = 3.0f;
    int direction = player.direction;

    Rectangle source;
    source.y = 0;
    source.width = 16;
    source.height = 16;

    if (direction == 1) {
        source.x = player.frame * 16;
    } else {
        source.x = (player.frame + 1) * 16; // ajuste para espelhamento
        source.width = -16; // espelha horizontalmente
    }

    Rectangle dest = { player.position.x, player.position.y, 16 * scale, 16 * scale };
    Vector2 origin = { 0, 0 };
    float rotation = 0.0f;

    DrawTexturePro(player.sprite, source, dest, origin, rotation, WHITE);
}


void free_player_resources() {
    UnloadTexture(player.sprite);
}
