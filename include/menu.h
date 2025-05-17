#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    MENU_PRINCIPAL,
    MENU_JOGANDO,
    MENU_PAUSADO,
    MENU_GAME_OVER
} EstadoMenu;

typedef struct {
    Rectangle rect;
    const char *texto;
    bool hover;
} Botao;

void init_menu();
void update_menu();
void draw_menu();
void unload_menu_textures();
EstadoMenu get_estado_menu();
void set_estado_menu(EstadoMenu estado);

#endif // MENU_H