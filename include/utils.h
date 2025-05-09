#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <raylib.h>

int clamp(int value, int min, int max);
float lerp(float a, float b, float t);
void delay_ms(int ms);
void log_debug(const char *msg);
bool check_collision(Rectangle a, Rectangle b);
void InitCamera();
void UpdateCameraPosition();

#endif // UTILS_H
