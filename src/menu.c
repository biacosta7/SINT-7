#include "../include/menu.h"
#include "player.h"
#include <stdio.h>


#define QUANT_BOTOES 3

static EstadoMenu estadoAtual = MENU_PRINCIPAL;
static Texture2D backgroundTexture;
static Texture2D logoTexture;
static Botao botoes[QUANT_BOTOES];
static Font fonte;

void init_menu() {
    // Carrega texturas
    backgroundTexture = LoadTexture("assets/menu/background.png");
    logoTexture = LoadTexture("assets/menu/logo.png");
    fonte = LoadFont("assets/menu/font.ttf"); // Ou use GetFontDefault()

    // Configura botões
    int btnWidth = 200;
    int btnHeight = 50;
    int startY = 300;
    int spacing = 20;

    const char* textos[] = {"Jogar", "Configurações", "Sair"};

    for (int i = 0; i < QUANT_BOTOES; i++) {
        botoes[i] = (Botao){
            .rect = {
                .x = (GetScreenWidth() - btnWidth) / 2,
                .y = startY + i * (btnHeight + spacing),
                .width = btnWidth,
                .height = btnHeight
            },
            .texto = textos[i],
            .hover = false
        };
    }
}

void update_menu() {
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < QUANT_BOTOES; i++) {
        botoes[i].hover = CheckCollisionPointRec(mousePos, botoes[i].rect);

        if (botoes[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            switch (i) {
                case 0: // Jogar
                    estadoAtual = MENU_JOGANDO;
                    init_player();
                    break;
                case 1: // Configurações
                    // Implemente conforme necessário
                    break;
                case 2: // Sair
                    CloseWindow();
                    break;
            }
        }
    }

    if (estadoAtual == MENU_JOGANDO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_PAUSADO;
    }
    else if (estadoAtual == MENU_PAUSADO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_JOGANDO;
    }
}

void draw_menu() {
    if (estadoAtual == MENU_PRINCIPAL) {
        // Desenha background
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        
        // Desenha logo
        DrawTexture(
            logoTexture,
            (GetScreenWidth() - logoTexture.width) / 2,
            100,
            WHITE
        );

        // Desenha botões
        for (int i = 0; i < QUANT_BOTOES; i++) {
            Color btnColor = botoes[i].hover ? SKYBLUE : LIGHTGRAY;
            DrawRectangleRec(botoes[i].rect, btnColor);
            
            // Centraliza texto nos botões
            int textWidth = MeasureText(botoes[i].texto, 20);
            DrawText(
                botoes[i].texto,
                botoes[i].rect.x + (botoes[i].rect.width - textWidth) / 2,
                botoes[i].rect.y + 15,
                20,
                DARKGRAY
            );
        }

        // Texto de copyright
        DrawText("© 2023 SINT-7 Game", 20, GetScreenHeight() - 30, 15, WHITE);
    }
    else if (estadoAtual == MENU_PAUSADO) {
        // Desenha overlay semi-transparente
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
        
        // Texto de pausado
        const char* texto = "PAUSADO";
        int textWidth = MeasureText(texto, 40);
        DrawText(
            texto,
            (GetScreenWidth() - textWidth) / 2,
            GetScreenHeight() / 2 - 50,
            40,
            WHITE
        );
        
        // Instrução
        const char* instrucao = "Pressione ESC para continuar";
        textWidth = MeasureText(instrucao, 20);
        DrawText(
            instrucao,
            (GetScreenWidth() - textWidth) / 2,
            GetScreenHeight() / 2 + 20,
            20,
            WHITE
        );
    }
}

void unload_menu_textures() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(logoTexture);
    UnloadFont(fonte);
}

EstadoMenu get_estado_menu() {
    return estadoAtual;
}

void set_estado_menu(EstadoMenu estado) {
    estadoAtual = estado;
}