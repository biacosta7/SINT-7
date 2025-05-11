#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>
#include "raylib.h" 
#include "config.h"

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

typedef struct Puzzle{
    bool foiSolucionado; // pra saber se o player coletou
    char *resposta;
    char *pergunta;
    int fase; // a qual fase ele pertence
    float x;
    float y;
    //Texture2D texture;
} Puzzle;

FragmentoMemoria fragmentoObrigatorioAtual;
Puzzle puzzleAtual;

typedef struct NodeFragmento {
    FragmentoMemoria fragmento;
    struct NodeFragmento *next;
} NodeFragmento;

extern NodeFragmento *fragmentosColetados;

extern FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS];
Puzzle puzzles[TOTAL_FRAGMENTOS_OBRIGATORIOS];

void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();

#endif // PUZZLES_H
