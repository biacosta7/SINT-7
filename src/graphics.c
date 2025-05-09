#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#define SECTOR_COUNT 2
#define SECTOR_WIDTH 900
#define SECTOR_HEIGHT 512
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Texture2D bgSectors[SECTOR_COUNT];
float cameraX = 0.0f;
extern Camera2D camera; 

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

void UpdateCameraMove() {
    cameraX = camera.target.x - SCREEN_WIDTH / 2;

    if (cameraX < 0) cameraX = 0;

    float maxCamera = (SECTOR_WIDTH * SECTOR_COUNT) - SCREEN_WIDTH;
    if (cameraX > maxCamera) cameraX = maxCamera;

    if (camera.target.x > 630){ // && não desbloqueou a fase 2 (fazer a mesma condicional p/outras fases, pode ser um loop de verificacao) - fase 1 limite : player.position.x = 1368 | camera.target.x = 678 | player pode andar até 1400 na primeira fase se a segunda nao tiver desbloqueada 
        cameraX = 630 - SCREEN_WIDTH / 2;
    }

    printf("cameraX: %f | camera.target.x: %f\n", cameraX, camera.target.x);
    
}


void DrawBackground() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        int sectorX = i * SECTOR_WIDTH;
        printf("sectorX: %d\n", sectorX);

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
