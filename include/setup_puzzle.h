#ifndef SETUP_PUZZLE_H
#define SETUP_PUZZLE_H
#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"

void init_puzzle(int fase);
void update_puzzle();
void draw_puzzle(int puzzle);
void free_puzzle_resources();

typedef struct Puzzle{
    bool foiSolucionado; // pra saber se o player coletou
    char *resposta;
    char *pergunta;
    int fase; // a qual fase ele pertence
    float x;
    float y;
    Texture2D texture;
} Puzzle;

Puzzle puzzleAtual;
extern bool puzzleFoiAtivado;

extern int countPuzzleCarregado;

Puzzle puzzles[TOTAL_FRAGMENTOS_OBRIGATORIOS];

#endif // SETUP_PUZZLE_H