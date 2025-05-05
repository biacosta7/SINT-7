#include "player.h"
#include <stdbool.h>

static Player player;  // Variável global interna ao módulo player.c

void init_player() {
    player.position = (Vector2){ 100, 100 };
    player.sprite = LoadTexture("assets/branquinho-fofinho/idle.png");
    player.vida = 3;
}

void update_player() {
    if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
    if (IsKeyDown(KEY_LEFT)) player.position.x -= 2;
}

void draw_player() {
    DrawTexture(player.sprite, (int)player.position.x, (int)player.position.y, WHITE);
}

void free_player_resources() {
    UnloadTexture(player.sprite);
}
