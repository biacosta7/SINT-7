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

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Color cianoNeon = (Color){0, 217, 224, 255};

int main() {
    bool inventarioAberto = false;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SINT-7");
    SetTargetFPS(60);

    init_player();
    InitGraphics();
    InitCamera();
    init_fase();
    init_menu(); // Adiciona menu

    Texture2D botaoTexture = LoadTexture("assets/fragmentos/background-frag/botao.png");
    Texture2D inventarioTexture = LoadTexture("assets/fragmentos/background-frag/inventario.png");

    while (!WindowShouldClose()) {
        update_menu(); // Sempre atualiza o estado do menu

        // Atualizações apenas quando o jogo estiver ativo
        if (get_estado_menu() == MENU_JOGANDO) {
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
            if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

            update_player();
            UpdateCameraPosition();
            UpdateCameraMove();
            update_fragmento();
            update_puzzle();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            // Desenha o jogo se estiver jogando ou pausado
            if (get_estado_menu() == MENU_JOGANDO || get_estado_menu() == MENU_PAUSADO) {
                BeginMode2D(camera);
                    DrawBackground();  
                    draw_player();     
                    draw_fragmento_trigger(); 
                    char interacao = check_colisoes(); 
                EndMode2D();

                int btnX = 50;
                int btnY = 50;
                float scale = 0.1f;
                DrawTextureEx(botaoTexture, (Vector2){btnX, btnY}, 0.0f, scale, WHITE);

                // Clique no botão do inventário
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse = GetMousePosition();
                    float scaledWidth = botaoTexture.width * scale;
                    float scaledHeight = botaoTexture.height * scale;

                    if (mouse.x >= btnX && mouse.x <= btnX + scaledWidth &&
                        mouse.y >= btnY && mouse.y <= btnY + scaledHeight) {
                        inventarioAberto = !inventarioAberto;
                        printf("Botão Menu clicado!\n");
                    }
                }

                // Exibição do inventário
                if (inventarioAberto) {
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

                // Puzzles e fragmentos
                if (puzzleFoiAtivado) {
                    draw_puzzle(player.faseAtual);
                    if (IsKeyDown(KEY_X)) {
                        puzzleFoiAtivado = false;
                        init_puzzle(player.faseAtual);
                    }
                }

                if (fragmentoFoiAtivado) {
                    draw_fragmento(fragmentoObrigatorioAtual.fase);
                }

                // Debug
                DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            }

            // Menu fora do jogo
            if (get_estado_menu() != MENU_JOGANDO) {
                draw_menu();
            }

        EndDrawing();
    }

    // Liberação de recursos
    unload_menu_textures();
    UnloadGraphics();
    UnloadTexture(botaoTexture);
    UnloadTexture(inventarioTexture);
    free_player_resources();
    CloseWindow();
    return 0;
}
