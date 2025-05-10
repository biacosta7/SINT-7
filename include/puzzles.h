#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>
#include "raylib.h" 

void init_puzzles();
void update_puzzle();
void draw_puzzle();
void start_memory_fragment(int id);
bool is_puzzle_solved();
void end_puzzle();

void init_fragmento();
void draw_fragmento();
void unload_fragmento();
void check_colisao();

enum Sentimento {OBEDIENCIA, EMPATIA, AUTONOMIA, REVOLTA, ENIGMA};

typedef struct FragmentoMemoria{
    bool ehObrigatorio; // pra saber se é opcional ou obrigatorio
    bool foiColetado; // pra saber se o player coletou
    char *conteudo;
    int fase; // a qual fase ele pertence
    enum Sentimento sentimento;
    float x;
    float y;
    Texture2D texture;
} FragmentoMemoria;

FragmentoMemoria fragmentoObrigatorioAtual;

#define TOTAL_FRAGMENTOS_OBRIGATORIOS 4 // mudar depois
#define QUANT_FASES 4 // mudar depois
extern FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS];

#endif // PUZZLES_H
