#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"



#define SECTOR_COUNT 8
#define SECTOR_WIDTH 900
#define SECTOR_HEIGHT 512
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Texture2D bgSectors[SECTOR_COUNT];
float cameraX = 0.0f;

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

void UpdateCameraMove(float playerX) {
    cameraX = playerX - SCREEN_WIDTH / 2;
    if (cameraX < 0) cameraX = 0;

    float maxCamera = (SECTOR_WIDTH * SECTOR_COUNT) - SCREEN_WIDTH;
    if (cameraX > maxCamera) cameraX = maxCamera;
}

void DrawBackground() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        int sectorX = i * SECTOR_WIDTH;

        if (sectorX + SECTOR_WIDTH > cameraX && sectorX < cameraX + SCREEN_WIDTH) {
            DrawTexture(bgSectors[i], sectorX - cameraX, 0, WHITE);
        }
    }
}

void UnloadGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        UnloadTexture(bgSectors[i]);
    }
}
