#ifndef FRAGMENTOS_H
#define FRAGMENTOS_H

#define NUM_FRAGMENTOS 4

#include "raylib.h" 
#include "config.h"
#include "player.h"

extern bool fragmentoFoiAtivado;
extern bool puzzleFoiAtivado;

typedef enum {
    OBEDIENCIA,
    EMPATIA,
    AUTONOMIA,
    REVOLTA,
    ENIGMA
} Sentimento;

typedef struct FragmentoMemoria{
    bool ehObrigatorio; // pra saber se é opcional ou obrigatorio
    bool foiColetado; // pra saber se o player coletou
    char *conteudo;
    int fase; // a qual fase ele pertence
    Sentimento sentimento;
    float x;
    float y;
    Texture2D texture;
    Texture2D trigger;
} FragmentoMemoria;

FragmentoMemoria fragmentoObrigatorioAtual;

typedef struct NodeFragmento {
    FragmentoMemoria fragmento;
    struct NodeFragmento *next;
} NodeFragmento;

extern NodeFragmento *fragmentosColetados;

extern int countFragCarregado;

extern FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS];

typedef struct {
    const char* conteudo;
    Sentimento sentimento;
} Fragmento;

void init_fragmento(int fase);
void update_fragmento();
void draw_fragmento(int fragmento);
void draw_fragmento_trigger();
bool check_colisao_fragmento(Rectangle playerHitbox);
bool check_colisao_puzzle(Rectangle playerHitbox);
void free_fragmento_resources();
void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();

static const Fragmento fragmentos[NUM_FRAGMENTOS] = {
    {"Fragmentos de memória revelam ordens seguidas cegamente, sem questionamentos.", OBEDIENCIA},
    {"Vestígios emocionais de dor e conexão são absorvidos, causando uma profunda mudança.", EMPATIA},
    {"Um lampejo de escolha, um momento de liberdade, um questionamento da programação.", AUTONOMIA},
    {"Registros de resistência e raiva contra a opressão são descobertos.", REVOLTA},
};

#endif // FRAGMENTOS_H
