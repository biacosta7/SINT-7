#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "config.h"

typedef enum {
    MENU_PRINCIPAL,
    MENU_LORE,       // Novo estado para a tela de lore
    MENU_JOGANDO,
    MENU_PAUSADO,
    MENU_GAME_OVER
} EstadoMenu;

typedef struct {
    Rectangle rect;
    const char *texto;
    bool hover;
} Botao;

// Estrutura para controlar a tela de lore
typedef struct {
    Texture2D background;
    const char *textoHistoria;
    float scrollOffset;
    float scrollSpeed;
    bool podePular;
    float tempoExibicao;
    Font fonte;
} TelaLore;

void init_menu();
void update_menu();
void draw_menu();
void unload_menu_textures();
EstadoMenu get_estado_menu();
void set_estado_menu(EstadoMenu estado);

#endif // MENU_H