#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h" 

typedef enum {
    IDLE,
    WALK
} PersonagemState;

// Estrutura do jogador
typedef struct Player {
    Vector2 position;
    Texture2D idleTexture;
    Texture2D walkTexture;
    PersonagemState state;
    int frame; //frame atual da animacao
    float frameTime; //tempo do frame
    int faseAtual;
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
