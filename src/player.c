#include "player.h"
#include <stdbool.h>

Player player;  // Variável global interna ao módulo player.c


float scale = 4.0f;  // Aumenta 4x (de 16x16 para 64x64 pixels)

void init_player() {
    player.position = (Vector2){ 100, 100 };
    player.sprite = LoadTexture("assets/branquinho-fofinho/idle.png");
    player.vida = 3;

    player.frame = 0;
    player.maxFrames = 3; // Supondo que sua spritesheet tenha 3 frames na linha
    player.frameTime = 0.1f;
    player.frameTimer = 0.0f;
}

void update_player() {

    player.frameTimer += GetFrameTime();
    if (player.frameTimer >= player.frameTime) {
        player.frameTimer = 0.0f;
        player.frame = (player.frame + 1) % player.maxFrames;  // Loop de 0 a 2
    }

    if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
    if (IsKeyDown(KEY_LEFT)) player.position.x -= 2;
}

void draw_player() {
    Rectangle source = {player.frame * 16, 0, 16, 16 };  // Frame original da spritesheet
    Rectangle dest = { player.position.x, player.position.y, 16 * scale, 16 * scale };  // Escala aplicada
    Vector2 origin = { 0, 0 };
    float rotation = 0.0f;

    DrawTexturePro(player.sprite, source, dest, origin, rotation, WHITE);
}

void free_player_resources() {
    UnloadTexture(player.sprite);
}
