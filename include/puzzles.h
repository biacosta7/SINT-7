#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>
#include "raylib.h" 
#include "config.h"

void init_puzzle(int fase);
void update_puzzle();
void draw_puzzles();
void draw_puzzle(int puzzle);
void start_memory_fragment(int id);
bool is_puzzle_solved();
void end_puzzle();
void puzzle_1();

void init_fragmento(int fase);
void draw_fragmento();
void unload_fragmento();
char check_colisoes();
bool check_colisao_fragmento(Rectangle playerHitbox);
bool check_colisao_puzzle(Rectangle playerHitbox);

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
    Texture2D texture;
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
