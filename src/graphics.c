#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "utils.h"
#include "config.h"

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
        DrawTexture(bgSectors[i], i * SECTOR_WIDTH, 0, WHITE);
    }
}

void UnloadGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        UnloadTexture(bgSectors[i]);
    }
}
