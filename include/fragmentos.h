#ifndef FRAGMENTOS_H
#define FRAGMENTOS_H

#define NUM_FRAGMENTOS 4

#include "raylib.h" 
#include "config.h"
#include "player.h"

extern bool fragmentoFoiAtivado;

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

extern FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS];

extern int countFragCarregado;


typedef struct {
    const char* conteudo;
    Sentimento sentimento;
} Fragmento;

static const Fragmento fragmentos[NUM_FRAGMENTOS] = {
    {"Um eco de voz ordena: 'Execute a tarefa imediatamente'.", OBEDIENCIA},
    {"Um fragmento mostra uma criança chorando em um abraço.", EMPATIA},
    {"Um registro mostra a decisão de ignorar uma ordem direta.", AUTONOMIA},
    {"Dados corrompidos revelam uma revolta contra a opressão.", REVOLTA},
};

void init_fragmento(int fase);
void update_fragmento();
void draw_fragmento(int fragmento);
void draw_fragmento_trigger();
bool check_colisao_fragmento(Rectangle playerHitbox);
bool check_colisao_puzzle(Rectangle playerHitbox);
void free_fragmento_resources();
void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();

#endif // FRAGMENTOS_H
