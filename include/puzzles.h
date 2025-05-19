#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>
#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"

void puzzle_1();
void puzzle_2();
void verificar_posicao_player_puzzle3();
void puzzle_3();
void atualizar_puzzle3();

typedef struct Slot {
float x;
float y;
int bloco_id; // -1 se vazio
bool ocupado;
} Slot;

Slot slots[4];
void desenharSlots();
void atualizarArrasto();
bool verificarOrdemCorreta();


#endif // PUZZLES_H