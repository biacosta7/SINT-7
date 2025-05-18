#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "setup_puzzle.h"
#include "puzzles.h"
#include "decode_puzzle.h"
#include "game.h"
#include "graphics.h"
#include "fase.h"
#include "fragmentos.h"
#include "ia.h"
#include "../include/menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Color cianoNeon = (Color){0, 217, 224, 255};
int scrollOffset = 0;

int main() {
    InitAudioDevice(); 
    //Music trilhaSonora = LoadMusicStream("assets/musica.ogg");
    //PlayMusicStream(trilhaSonora); adicionar a trilha sonora e colocar ela no parenteses a cima

    bool inventarioAberto = false;
    bool aba_comandos_aberto = false;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SINT-7");
    SetTargetFPS(60);

    init_player();
    InitGraphics();
    InitCamera();
    init_fase();
    init_menu();

    Texture2D botaoTexture = LoadTexture("assets/fragmentos/background-frag/botao.png");
    Texture2D inventarioTexture = LoadTexture("assets/fragmentos/background-frag/inventario.png");

    for (int i = 0; i < 4; i++) {
        printf("$OI AQUI: Conteúdo: %s | Sentimento: %d\n", fragmentos[i].conteudo, fragmentos[i].sentimento);
    }

    while (!WindowShouldClose()) {
        //UpdateMusicStream(trilhaSonora);
        update_menu();

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

        if (get_estado_menu() == MENU_JOGANDO || get_estado_menu() == MENU_PAUSADO) {
            BeginMode2D(camera);
            DrawBackground();
            draw_player();
            draw_fragmento_trigger();
            char interacao = check_colisoes();
            atualizar_e_desenhar_fundo_escuro();
            EndMode2D();
            puzzle_decode();

            int btnX = 50;
            int btnY = 50;
            float scale = 0.1f;
            float scaledWidth = botaoTexture.width * scale;
            float scaledHeight = botaoTexture.height * scale;

            Vector2 btnInventarioPos = {btnX, btnY};
            Vector2 btnComandosPos = {btnX + scaledWidth + 10, btnY};

            DrawTextureEx(botaoTexture, btnInventarioPos, 0.0f, scale, WHITE);
            DrawTextureEx(botaoTexture, btnComandosPos, 0.0f, scale, WHITE);

            if(deveDesenharEscuro) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                              (Color){0, 0, 0, (unsigned char)escurecimentoAlpha});
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();

                if (mouse.x >= btnInventarioPos.x && mouse.x <= btnInventarioPos.x + scaledWidth &&
                    mouse.y >= btnInventarioPos.y && mouse.y <= btnInventarioPos.y + scaledHeight) {
                    inventarioAberto = !inventarioAberto;
                    printf("Botão Inventário clicado!\n");
                } else if (mouse.x >= btnComandosPos.x && mouse.x <= btnComandosPos.x + scaledWidth &&
                           mouse.y >= btnComandosPos.y && mouse.y <= btnComandosPos.y + scaledHeight) {
                    aba_comandos_aberto = !aba_comandos_aberto;
                    printf("Botão Comandos clicado!\n");
                }
            }

            if (inventarioAberto) {
                int scrollSpeed = 20;
                if (IsKeyDown(KEY_DOWN)) scrollOffset -= scrollSpeed;
                if (IsKeyDown(KEY_UP)) scrollOffset += scrollSpeed;
                scrollOffset += GetMouseWheelMove() * scrollSpeed;
                if (scrollOffset > 0) scrollOffset = 0;

                int scaleI = 5.0f;
                int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
                int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;

                DrawTextureEx(inventarioTexture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);

                int i = 1;
                int textoX = inventX + 40;
                int textoY = inventY + 50 + scrollOffset;
                int linhaAltura = 25;
                int textoAlturaTotal = 0;
                NodeFragmento *atual = fragmentosColetados;

                BeginScissorMode(inventX, inventY + 30, inventarioTexture.width * scaleI, inventarioTexture.height * scaleI);
                while (atual != NULL) {
                    textoAlturaTotal += linhaAltura;
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
                int inventarioAltura = inventarioTexture.height * scaleI;
                int scrollLimiteInferior = inventarioAltura - textoAlturaTotal - 50;
                if (scrollOffset < scrollLimiteInferior){
                    scrollOffset = scrollLimiteInferior;
                }
                EndScissorMode();
            }

            if (aba_comandos_aberto) {
                int imgX = (SCREEN_WIDTH - inventarioTexture.width) / 2;
                int imgY = (SCREEN_HEIGHT - inventarioTexture.height) / 2;

                int scaleI = 5.0f;
                int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
                int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;
                DrawTextureEx(inventarioTexture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);

            }

            if (puzzleFoiAtivado) {
                draw_puzzle(player.faseAtual);
                if (IsKeyDown(KEY_X)) {
                    puzzleFoiAtivado = false;
                    init_puzzle(player.faseAtual);
                    alternar_estado_fundo_escuro(false);
                }
            }

            if (fragmentoFoiAtivado) {
                draw_fragmento(fragmentoObrigatorioAtual.fase);
            }

            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
        }

        if (get_estado_menu() != MENU_JOGANDO) {
            draw_menu();
        }
        EndDrawing();
    }
    //UnloadMusicStream(trilhaSonora);
    //CloseAudioDevice(); 
    unload_menu_textures();
    UnloadGraphics();
    UnloadTexture(botaoTexture);
    UnloadTexture(inventarioTexture);
    free_player_resources();
    CloseWindow();
    return 0;
}
