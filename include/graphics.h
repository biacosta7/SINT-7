#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "stdbool.h"

void InitGraphics();
void UnloadGraphics();
void DrawBackground();
void atualizarMaiorFase();
void atualizar_e_desenhar_fundo_escuro();
void alternar_estado_fundo_escuro(bool aberto);
void DrawTextoInteracaoComFundo(float textX, float textY);
void QuebrarTextoPorLargura(const char *textoOriginal, char *textoFormatado, int larguraMax, int tamanhoFonte);

extern bool escurecendo;
extern bool deveDesenharEscuro;
extern bool clareando;
extern float escurecimentoAlpha; // Opacidade atual

extern float cameraX;

#endif
