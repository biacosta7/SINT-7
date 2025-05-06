#include "graphics.h"
#include "raylib.h"
#include "player.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Variáveis globais ou estáticas para o fundo
static Texture2D background;

void InitGraphics() {
    background = LoadTexture("assets/map-background.png");
}

void UnloadGraphics() {
    UnloadTexture(background);
}

void DrawBackground(Vector2 playerPosition) {
    int cameraX = (int)(playerPosition.x - SCREEN_WIDTH / 2);
    if (cameraX < 0) cameraX = 0;

    Rectangle source = { cameraX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    Vector2 position = { 0, 0 };

    DrawTextureRec(background, source, position, WHITE);
}
