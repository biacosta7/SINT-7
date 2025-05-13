#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h" 
#include "config.h"

typedef enum {
    IDLE,
    WALK
} PersonagemState;

typedef struct Player {
    Vector2 position;
    Texture2D idleTexture;
    Texture2D walkTexture;
    PersonagemState state;
    int frame;
    float frameTime;
    int faseAtual;
    int width;
    int maxFrames;
    bool flipRight; 
    int x;
    int y;
    int height;
    float frameTimer;
    int direction;
    bool fasesDesbloqueadas[QUANT_FASES];
} Player;

extern Player player;

void init_player();
void update_player();
void draw_player();
void free_player_resources();
void desbloquear_fase(int fase);

#endif // PLAYER_H