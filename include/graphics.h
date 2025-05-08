#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

void InitGraphics();
void UnloadGraphics();
void DrawBackground();
void UpdateCameraMove(float playerX);

extern float cameraX;

#endif
