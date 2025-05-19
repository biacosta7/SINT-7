#ifndef DECODE_PUZZLE_H
#define DECODE_PUZZLE_H

#include "config.h"
#include "raylib.h"
#include <stdbool.h>

#define SEQ_LENGTH 7
#define ROW_SIZE SEQ_LENGTH
#define TOTAL_TIME 30.0f

typedef enum PuzzleState {
    PS_Initializing,
    PS_Countdown,
    PS_DisplaySequence,
    PS_Selecting,
    PS_Success,
    PS_Failure
} PuzzleState;

typedef struct {
    const char* text;
    float delayPerChar;
    float timer;
    int visibleChars;
    bool completed;
} TypewriterText;

void UpdateTypewriterText(TypewriterText *tw, float delta);
void DrawTypewriterText(TypewriterText *tw, int x, int y, int size, Color color);
void puzzle_decode();

#endif