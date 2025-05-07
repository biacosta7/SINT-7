#include "graphics.h"
#include "raylib.h"
#include "player.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 512

// Variáveis globais ou estáticas para o fundo
static Texture2D background;

void InitGraphics() {
    background = LoadTexture("assets/testemap.png");
    if (background.id == 0) {
        TraceLog(LOG_ERROR, "Erro ao carregar o fundo");
    }
}


void UnloadGraphics() {
    UnloadTexture(background);
}

void DrawBackground() {
    DrawTexture(background, SCREEN_WIDTH/2 - 32000/2, SCREEN_HEIGHT/2 - 1350/2, WHITE);
}

