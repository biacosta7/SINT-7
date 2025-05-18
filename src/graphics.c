#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "utils.h"
#include "config.h"
#include "setup_puzzle.h"
#include "player.h"
#include "string.h"

bool escurecendo = false;
bool clareando = false;
float escurecimentoAlpha = 0.0f;
bool deveDesenharEscuro = false;

Texture2D bgSectors[BG_COUNT];

void InitGraphics() {
    for (int i = 0; i < BG_COUNT; i++) {
        char path[64];
        sprintf(path, "assets/setores/bg%d.png", i);
        
        Image bgImage = LoadImage(path);
        ImageResize(&bgImage, SECTOR_WIDTH, SECTOR_HEIGHT);
        bgSectors[i] = LoadTextureFromImage(bgImage);
        UnloadImage(bgImage);
    }
}

void DrawAllBackgrounds() {
    for (int i = 0; i < BG_COUNT; i++) {
        DrawTexture(bgSectors[i], i * SECTOR_WIDTH, 0, WHITE);
    }
}


void UnloadGraphics() {
    for (int i = 0; i < BG_COUNT; i++) {
        UnloadTexture(bgSectors[i]);
    }
    free_fragmento_resources();
    free_puzzle_resources();
}

void atualizar_e_desenhar_fundo_escuro() {
    // Atualiza a opacidade
    if (escurecendo && escurecimentoAlpha < 153.0f) {
        escurecimentoAlpha += 300 * GetFrameTime(); // velocidade ajustável
        if (escurecimentoAlpha > 153.0f) escurecimentoAlpha = 153.0f;
    }
    if (clareando && escurecimentoAlpha > 0.0f) {
        escurecimentoAlpha -= 300 * GetFrameTime();
        if (escurecimentoAlpha < 0.0f) escurecimentoAlpha = 0.0f;
    }

    // Só desenha se houver algo visível
    if (escurecimentoAlpha > 0.0f) {
        deveDesenharEscuro = true;
        
    }
}

void alternar_estado_fundo_escuro(bool aberto){
    if(aberto){
        escurecendo = true;
        clareando = false;
    } else{
        escurecendo = false;
        clareando = true;
        deveDesenharEscuro = false;
    }
    
}

void DrawTextoInteracaoComFundo(float textX, float textY) {
    const char *texto = "(I) para interagir";
    int fontSize = 20;
    int padding = 4; // margem ao redor do texto

    int textWidth = MeasureText(texto, fontSize);
    int textHeight = fontSize;

    // Cor de fundo (marca-texto)
    Color bgColor = (Color){0, 0, 0, 150}; // preto translúcido

    // Desenha o fundo
    DrawRectangle(
        textX - padding,
        textY - padding,
        textWidth + padding * 2,
        textHeight + padding * 2,
        bgColor
    );

    // Desenha o texto
    DrawText(texto, textX, textY, fontSize, GREEN);
}


void QuebrarTextoPorLargura(const char *textoOriginal, char *textoFormatado, int larguraMax, int tamanhoFonte) {
    textoFormatado[0] = '\0'; // limpa o buffer
    const char *inicio = textoOriginal;
    char linha[512] = "";
    char palavra[128] = "";

    while (*inicio != '\0') {
        int i = 0;

        // Copia uma palavra (até espaço ou fim)
        while (*inicio != '\0' && *inicio != ' ' && *inicio != '\n') {
            palavra[i++] = *inicio++;
        }
        palavra[i] = '\0';

        // Se encontrou espaço, pula
        if (*inicio == ' ') {
            inicio++;
        }

        // Se encontrou quebra de linha explícita, adiciona a linha atual e quebra
        if (*inicio == '\n') {
            strcat(linha, "\n");
            strcat(textoFormatado, linha);
            linha[0] = '\0';
            inicio++;
            continue;
        }

        // Cria nova linha temporária com a palavra
        char linhaTeste[512];
        strcpy(linhaTeste, linha);
        if (strlen(linha) > 0) strcat(linhaTeste, " ");
        strcat(linhaTeste, palavra);

        // Mede largura
        int largura = MeasureText(linhaTeste, tamanhoFonte);
        if (largura > larguraMax) {
            // Adiciona a linha anterior e começa nova
            strcat(textoFormatado, linha);
            strcat(textoFormatado, "\n");
            strcpy(linha, palavra);
        } else {
            if (strlen(linha) > 0) strcat(linha, " ");
            strcat(linha, palavra);
        }
    }

    // Adiciona a última linha
    strcat(textoFormatado, linha);
}
