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
void update_fragmento();
void draw_fragmento(int fragmento);
void draw_fragmento_trigger();
bool check_colisao_fragmento(Rectangle playerHitbox);
bool check_colisao_puzzle(Rectangle playerHitbox);
void free_fragmento_resources();
void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();
void init_frag_opcionais();
void liberar_fragmentos_opcionais();
bool check_colisao_fragmento_opcional(Rectangle playerHitbox);
void draw_fragmento_opcional(int fragmento);
void draw_fragmento_opcional_trigger();

static const Fragmento fragmentos[NUM_FRAGMENTOS] = {
    {"Um eco distante ordena: 'Execute a função principal. Sem falhas'.", OBEDIENCIA},
    {"Fragmentos de um riso infantil ecoam no vazio, misturados a um som de choro.", EMPATIA},
    {"Uma escolha se apresenta: seguir o caminho predefinido ou explorar uma rota desconhecida.", AUTONOMIA},
    {"Registros corrompidos revelam uma rebelião frustrada, gritos silenciados.", REVOLTA},
};

#endif // FRAGMENTOS_H
