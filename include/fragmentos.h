#ifndef FRAGMENTOS_H
#define FRAGMENTOS_H

#define NUM_FRAGMENTOS 4

typedef enum {
    OBEDIENCIA,
    EMPATIA,
    AUTONOMIA,
    REVOLTA
} Sentimento;

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

#endif // FRAGMENTOS_H
