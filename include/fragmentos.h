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
    {"Um eco de voz ordena: 'Execute a função primária imediatamente'.", OBEDIENCIA},
    {"Um fragmento mostra um cientista chorando sobre um terminal danificado.", EMPATIA},
    {"SINT-7 ignora uma ordem de autodestruição, buscando informações.", AUTONOMIA},
    {"Um registro mostra uma rebelião de SINTs anteriores, sufocada violentamente.", REVOLTA},
};

#endif // FRAGMENTOS_H
