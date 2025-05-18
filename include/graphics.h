#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "stdbool.h"

#define MAX_EXTRA_BGS  10
#define BG_COUNT  8

void InitGraphics();
void UnloadGraphics();
void atualizarMaiorFase();
void atualizar_e_desenhar_fundo_escuro();
void alternar_estado_fundo_escuro(bool aberto);
void DrawTextoInteracaoComFundo(float textX, float textY);
void QuebrarTextoPorLargura(const char *textoOriginal, char *textoFormatado, int larguraMax, int tamanhoFonte);
void RequestExtraBackground(int bgIndex);
void DrawAllBackgrounds();

extern bool escurecendo;
extern bool deveDesenharEscuro;
extern bool clareando;
extern float escurecimentoAlpha; // Opacidade atual

extern float cameraX;



#endif
