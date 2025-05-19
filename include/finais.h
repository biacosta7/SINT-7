#ifndef FINAIS_H
#define FINAIS_H
#include <raylib.h>

typedef struct {
    char nome[20]; // Ex: "empatia"
    int valor;
} Contador;



typedef struct Node {
    Contador dado;
    struct Node* left;
    struct Node* right;
} Node;

extern Node* raiz_arvore_sentimentos;

Node* criar_no(char* nome, int valor);
Node* inserir(Node* raiz, char* nome, int valor);
Contador encontrar_maior(Node* raiz);
void incrementar_contador(Node* raiz, const char* nome);
void mostrar_final(Node* raiz);

#endif 