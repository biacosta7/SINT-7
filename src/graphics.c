#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "utils.h"

#define SECTOR_COUNT 2
#define SECTOR_WIDTH 900
#define SECTOR_HEIGHT 512
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Texture2D bgSectors[SECTOR_COUNT];

void InitGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        char path[64];
        sprintf(path, "assets/setores/bg%d.png", i);

        Image bgImage = LoadImage(path);
        ImageResize(&bgImage, SECTOR_WIDTH, SECTOR_HEIGHT);
        bgSectors[i] = LoadTextureFromImage(bgImage);
        UnloadImage(bgImage);
    }
}


void DrawBackground() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        int sectorX = i * SECTOR_WIDTH;

        int preloadMargin = 100;  // margem de pré-carregamento

        if (sectorX + SECTOR_WIDTH > cameraX - preloadMargin &&
            sectorX < cameraX + SCREEN_WIDTH + preloadMargin)
        {
            DrawTexture(bgSectors[i], sectorX - cameraX, 0, WHITE);
        }
    }
}

void UnloadGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        UnloadTexture(bgSectors[i]);
    }
}
