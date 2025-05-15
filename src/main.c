#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "puzzles.h"
#include "graphics.h"
#include "fase.h"
#include "../include/menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Color cianoNeon = (Color){0, 217, 224, 255};

int main() {
    carregar_fragmentos("fragmentos_processados.json");

    bool inventarioAberto = false;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SINT-7");
    SetTargetFPS(60);
    
    InitGraphics();
    InitCamera();
    init_menu(); // Inicializa o menu primeiro

    // Carrega texturas do inventário
    Texture2D botaoTexture = LoadTexture("assets/fragmentos/background-frag/botao.png");
    Texture2D inventarioTexture = LoadTexture("assets/fragmentos/background-frag/inventario.png");

    while (!WindowShouldClose()) {
        // Atualiza o estado do menu/jogo
        update_menu();

        // Lógica do jogo (apenas quando estiver jogando)
        if (get_estado_menu() == MENU_JOGANDO) {
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
            if (IsKeyDown(KEY_LEFT)) player.position.x -= 2;

            update_player();
            UpdateCameraPosition();
            UpdateCameraMove();
            update_fragmento();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            // Desenha o jogo (se não estiver no menu principal)
            if (get_estado_menu() == MENU_JOGANDO || get_estado_menu() == MENU_PAUSADO) {
                BeginMode2D(camera);
                    DrawBackground();
                    draw_player();
                    draw_fragmento_trigger();
                    char interacao = check_colisoes();
                EndMode2D();

                // Interface do jogo
                int btnX = 50;
                int btnY = 50;
                float scale = 0.1f;
                DrawTextureEx(botaoTexture, (Vector2){btnX, btnY}, 0.0f, scale, WHITE);

                // Controle do inventário
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    Vector2 mouse = GetMousePosition();
                    if (mouse.x >= btnX && mouse.x <= btnX + botaoTexture.width &&
                        mouse.y >= btnY && mouse.y <= btnY + botaoTexture.height) {
                        inventarioAberto = !inventarioAberto;
                    }
                }

                if(inventarioAberto) {
                    int scaleI = 5.0f;
                    int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
                    int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;
                    
                    DrawTextureEx(inventarioTexture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);
                    
                    int i = 1;
                    int textoX = inventX + 40;
                    int textoY = inventY + 50;
                    int linhaAltura = 25;
                    NodeFragmento *atual = fragmentosColetados;
                    
                    while (atual != NULL) {
                        DrawText(TextFormat("FM-00%d:", i), textoX, textoY, 20, cianoNeon);
                        textoY += linhaAltura;
                        char buffer[512];
                        strcpy(buffer, atual->fragmento.conteudo);
                        char *linha = strtok(buffer, "\n");
                        while (linha != NULL) {
                            DrawText(linha, textoX + 10, textoY, 20, WHITE);
                            textoY += linhaAltura;
                            linha = strtok(NULL, "\n");
                        }
                        textoY += 10;
                        atual = atual->next;
                        i++;
                    }
                }

                // Interações com puzzles e fragmentos
                if(interacao == 'p') draw_puzzle(puzzleAtual.fase);
                if (fragmentoFoiAtivado) {
                    draw_fragmento(fragmentoObrigatorioAtual.fase);
                }
                if (interacao == 'o') {
                    draw_fragmento(fragmentoOpcionalAtual.fase);
                }
                // Debug info
                DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            }

            // Desenha o menu (se necessário)
            if (get_estado_menu() != MENU_JOGANDO) {
                draw_menu();
            }

        EndDrawing();
    }

    // Libera recursos
    unload_menu_textures();
    UnloadGraphics();
    UnloadTexture(botaoTexture);
    UnloadTexture(inventarioTexture);
    free_player_resources();
    CloseWindow();
    
    return 0;
}