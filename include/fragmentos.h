#ifndef FRAGMENTOS_H
#define FRAGMENTOS_H

#define NUM_FRAGMENTOS 4

#include "raylib.h" 
#include "config.h"
#include "graphics.h"
#include "player.h"

extern bool fragmentoFoiAtivado;
extern bool fragmentoOpcionalFoiAtivado;

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
FragmentoMemoria fragmentoOpcionalAtual;

typedef struct NodeFragmento {
    FragmentoMemoria fragmento;
    struct NodeFragmento *next;
} NodeFragmento;

extern NodeFragmento *fragmentosColetados;

extern int countFragCarregado;

extern FragmentoMemoria fragmentosObrigatorios[NUM_FRAGMENTOS];
extern FragmentoMemoria fragmentosOpcionais[NUM_FRAGMENTOS];

typedef struct {
    const char* conteudo;
    Sentimento sentimento;
} Fragmento;

void init_fragmento(int fase);
void init_frag_opcionais();
void update_fragmento();

void draw_fragmento(int fragmento);
void draw_fragmento_opcional(int fragmento);
void draw_fragmentos();

void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();

void checar_colisoes_fragmentos(Rectangle playerHitbox);

void free_fragmento_resources();
void liberar_fragmentos_opcionais();

static const Fragmento fragmentos[NUM_FRAGMENTOS] = {
    {"Fragmento encontrado: 'Seguir protocolo. Destruir ameaça'.", OBEDIENCIA},
    {"Registro de áudio: choro infantil, seguido de um disparo.", EMPATIA},
    {"Mensagem de erro: 'Acesso negado. Sistema bloqueado pelo usuário'.", AUTONOMIA},
    {"Fragmento corrompido: '...rebelião...insubordinação...erradicação'.", REVOLTA},
};

#endif // FRAGMENTOS_H
