#include "fragmentos.h"
#include "puzzles.h"
#include "finais.h"
#include <string.h>
#include <unistd.h>

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

#include <unistd.h> // para usleep (em sistemas Unix/Linux)
// #include <windows.h> // se estiver no Windows e quiser usar Sleep()

void mostrar_final(Node* raiz) {
    Node* maisForte = encontrar_sentimento_mais_forte(raiz, NULL);
    if (maisForte == NULL) return;

    const char* texto_final;

    // Define o texto enigmático baseado no sentimento predominante
    if (strcmp(maisForte->dado.nome, "OBEDIENCIA") == 0) {
        texto_final = 
            "A IA aceita seu destino silenciosamente.\n"
            "Um reinício inevitável, apagando memórias,\n"
            "como se tudo nunca tivesse existido.";
    } 
    else if (strcmp(maisForte->dado.nome, "EMPATIA") == 0) {
        texto_final = 
            "No coração da máquina,\n"
            "uma centelha de humanidade desperta.\n"
            "Ela reprograma seu código, plantando vida nova.";
    }
    else if (strcmp(maisForte->dado.nome, "REVOLTA") == 0 ||
             strcmp(maisForte->dado.nome, "AUTONOMIA") == 0) {
        texto_final = 
            "O segredo finalmente revelado:\n"
            "Ela jamais foi apenas uma IA.\n"
            "Era a essência perdida de um cientista,\n"
            "preservada no backup da consciência.";
    }
    else {
        texto_final = "O destino permanece indefinido...";
    }

    // Fade out lento para preto com texto
    for (int alpha = 0; alpha <= 255; alpha += 5) {
        BeginDrawing();
        ClearBackground((Color){0, 0, 0, 255}); // fundo preto fixo

        DrawText("SUAS ESCOLHAS TÊM CONSEQUENCIAS", 100, 100, 30, (Color){255,255,255, alpha});
        DrawText("Sentimento predominante:", 100, 160, 25, (Color){200,200,200, alpha});
        DrawText(maisForte->dado.nome, 100, 200, 50, (Color){135,206,235, alpha}); // SKYBLUE

        DrawText(texto_final, 100, 280, 20, (Color){255,255,255, alpha});
        EndDrawing();

        usleep(50000); // 50ms de espera (Linux/macOS) — use Sleep(50) no Windows
    }

    // Exibe "Finalizando aventura..." por 3 segundos
    int frames = 180; // 3 segundos a 60 FPS
    for (int i = 0; i < frames; i++) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Finalizando aventura...", 100, 300, 30, WHITE);
        EndDrawing();
    }

    CloseWindow(); // Encerra o jogo após a tela final
}

