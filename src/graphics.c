#include "graphics.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "utils.h"
#include "config.h"
#include "setup_puzzle.h"
#include "player.h"

bool escurecendo = false;
bool clareando = false;
float escurecimentoAlpha = 0.0f;
bool deveDesenharEscuro = false;

Texture2D bgSectors[SECTOR_COUNT];

void InitGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        char path[64];
        sprintf(path, "assets/setores/bg%d.png", i);
        
        Image bgImage = LoadImage(path);
        ImageResize(&bgImage, SECTOR_WIDTH, SECTOR_HEIGHT);
        bgSectors[i] = LoadTextureFromImage(bgImage);
        UnloadImage(bgImage);
    }
}

int maiorFaseAlcancada = 1; // começa na fase 1

void atualizarMaiorFase() {
    if (player.faseAtual > maiorFaseAlcancada) {
        maiorFaseAlcancada = player.faseAtual;
    }
}

void DrawBackground() {
    atualizarMaiorFase();

    int maxIndex = (maiorFaseAlcancada * 2); // cada fase tem 2 bgs

    for (int i = 0; i < maxIndex && i < SECTOR_COUNT; i++) {
        DrawTexture(bgSectors[i], i * SECTOR_WIDTH, 0, WHITE);
    }
}

void UnloadGraphics() {
    for (int i = 0; i < SECTOR_COUNT; i++) {
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