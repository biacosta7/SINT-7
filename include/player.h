#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"  // Necessário se usar tipos da Raylib, como Texture2D

// Estrutura do jogador
typedef struct Player {
    Vector2 position;
    Texture2D sprite;
    int frame; //frame atual da animacao
    float frameTime; //tempo do frame
    int vida;
    int width;
    int maxFrames;
    bool flipRight; 
    int x;
    int y;
    int height;
    float frameTimer; //acumulador de tempo
    int direction;
} Player;

extern Player player; // <- Isso torna a variável visível fora do player.c

// Funções relacionadas ao jogador
void init_player();
void update_player();
void draw_player();
void free_player_resources();

#endif // PLAYER_H
