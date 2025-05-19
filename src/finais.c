#include "fragmentos.h"
#include "puzzles.h"
#include "finais.h"

Node* raiz_arvore_sentimentos = NULL;

Node* criar_no(char* nome, int valor) {
    Node* novo = malloc(sizeof(Node));
    strcpy(novo->dado.nome, nome);
    novo->dado.valor = valor;
    novo->left = NULL;
    novo->right = NULL;
    return novo;
}

Node* inserir(Node* raiz, char* nome, int valor) {
    if (raiz == NULL) {
        return criar_no(nome, valor);
    }

    if (valor < raiz->dado.valor)
        raiz->left = inserir(raiz->left, nome, valor);
    else
        raiz->right = inserir(raiz->right, nome, valor);

    return raiz;
}

Node* encontrar_sentimento_mais_forte(Node* raiz, Node* maisForte) {
    if (raiz == NULL) return maisForte;

    if (maisForte == NULL || raiz->dado.valor > maisForte->dado.valor) {
        maisForte = raiz;
    }

    maisForte = encontrar_sentimento_mais_forte(raiz->left, maisForte);
    maisForte = encontrar_sentimento_mais_forte(raiz->right, maisForte);

    return maisForte;
}


void incrementar_contador(Node* raiz, const char* nome) {
    if (raiz == NULL) return;

    int comparacao = strcmp(nome, raiz->dado.nome);

    if (comparacao == 0) {
        raiz->dado.valor++;
    } else if (comparacao < 0) {
        incrementar_contador(raiz->left, nome);
    } else {
        incrementar_contador(raiz->right, nome);
    }
}

void mostrar_final(Node* raiz) {
    Node* maisForte = encontrar_sentimento_mais_forte(raiz, NULL);

    if (maisForte == NULL) return;

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("FINAL BASEADO EM SUAS ESCOLHAS", 100, 100, 30, WHITE);
    DrawText("Sentimento predominante:", 100, 160, 25, LIGHTGRAY);
    DrawText(maisForte->dado.nome, 100, 200, 50, SKYBLUE);

    EndDrawing();
}
