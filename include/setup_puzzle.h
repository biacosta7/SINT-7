#ifndef SETUP_PUZZLE_H
#define SETUP_PUZZLE_H
#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"

void init_puzzle(int fase);
void update_puzzle();
void draw_puzzle(int puzzle);
void free_puzzle_resources();
void verifica_puzzle_interativo();
bool check_colisao_puzzle(Rectangle playerHitbox);
void carregarBlocos();
void drawBlocos();
bool checar_colisao_blocos(Rectangle playerHitbox);

typedef struct Puzzle{
    bool foiSolucionado; // pra saber se o player coletou
    char *resposta;
    char *pergunta;
    int fase; // a qual fase ele pertence
    float x;
    float y;
    Texture2D texture;
} Puzzle;

typedef struct Bloco{
    int num;
    bool foiColetado;
    float x;
    float y;
    Texture2D texture;
} Bloco;

Puzzle puzzleAtual;
extern bool puzzleFoiAtivado;
extern Bloco blocos[4];
extern int countPuzzleCarregado;
Texture2D maquina;

Puzzle puzzles[NUM_FRAGMENTOS];

extern bool blocoFoiAtivado;
Bloco blocoAtual;

#endif // SETUP_PUZZLE_H