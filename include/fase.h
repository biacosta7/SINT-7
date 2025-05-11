#ifndef FASE_H
#define FASE_H
#include "puzzles.h"

void init_fase();
void update_fase();
void draw_fase();
void reset_fase();

typedef struct {
    int numero;
    bool foiConcluida;
    Puzzle puzzle;
    FragmentoMemoria fragmentoObrigatorio;
} Fase;

Fase faseAtual;

#endif // FASE_H
