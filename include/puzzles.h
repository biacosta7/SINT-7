#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>
#include "raylib.h" 
#include "config.h"
void update_fragmento_opcional();
void init_puzzle(int fase);
void update_puzzle();
void draw_puzzle(int puzzle);
void start_memory_fragment(int id);
bool is_puzzle_solved();
void end_puzzle();
void puzzle_1();

void init_fragmento(int fase);
void update_fragmento();
void draw_fragmento(int fragmento);
void draw_fragmento_trigger();
char check_colisoes();
bool check_colisao_fragmento_opcional(Rectangle playerHitbox);
bool check_colisao_fragmento(Rectangle playerHitbox);
bool check_colisao_puzzle(Rectangle playerHitbox);
void free_fragmento_resources();
void free_puzzle_resources();

extern bool fragmentoFoiAtivado;

enum Sentimento {OBEDIENCIA, EMPATIA, AUTONOMIA, REVOLTA, ENIGMA, SENTIMENTO_NULO};

typedef struct FragmentoMemoria{
    bool ehObrigatorio; // pra saber se é opcional ou obrigatorio
    bool foiColetado; // pra saber se o player coletou
    char *conteudo;
    int fase; // a qual fase ele pertence
    enum Sentimento sentimento;
    float x;
    float y;
    Texture2D texture;
    Texture2D trigger;
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
FragmentoMemoria fragmentoOpcionalAtual;

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
