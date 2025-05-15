#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "puzzles.h"
#include "graphics.h"
#include "fase.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Color cianoNeon = (Color){0, 217, 224, 255};

int main() {
    bool inventarioAberto = false;
    InitWindow(900, 512, "SINT-7");
    SetTargetFPS(60);
    
    init_player();
    InitGraphics();
    InitCamera();
    init_fase();

    Texture2D botaoTexture = LoadTexture("assets/fragmentos/background-frag/botao.png");
    Texture2D inventarioTexture = LoadTexture("assets/fragmentos/background-frag/inventario.png");

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
        if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

        update_player(); // Atualiza jogador
        UpdateCameraPosition(); // Atualiza camera.target.x se necessário
        UpdateCameraMove(); // Atualiza cameraX baseado na nova camera
        update_fragmento();
        update_puzzle();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                DrawBackground();  // DESENHA OS SETORES
                draw_player();     // DESENHA O PLAYER
                draw_fragmento_trigger(); // DESENHA O TRIGGER FRAGMENTO
                char interacao = check_colisoes(); // Chamar apenas uma vez

            EndMode2D();

            int btnX = 50;
            int btnY = 50;
            int scaleI = 5.0f;
            int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
            int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;
            float scale = 0.1f;
            DrawTextureEx(botaoTexture, (Vector2){btnX, btnY}, 0.0f, scale, WHITE);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                Vector2 mouse = GetMousePosition();
                if (mouse.x >= btnX && mouse.x <= btnX + botaoTexture.width &&
                    mouse.y >= btnY && mouse.y <= btnY + botaoTexture.height) {
                    inventarioAberto = !inventarioAberto;
                    printf("Botão Menu clicado!\n");
                }
            }
            if(inventarioAberto) {
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
                    strcpy(buffer, atual->fragmento.conteudo); // Para não alterar o original
                    char *linha = strtok(buffer, "\n");
                    while (linha != NULL) {
                        DrawText(linha, textoX + 10, textoY, 20, WHITE); // Indentação leve
                        textoY += linhaAltura;
                        linha = strtok(NULL, "\n");
                    }
                    textoY += 10; // Espaço extra entre fragmentos
                    atual = atual->next;
                    i++;
                }
            }
            if (puzzleFoiAtivado) {
                draw_puzzle(player.faseAtual);

                // ESC fecha o puzzle
                if (IsKeyDown(KEY_X)) {
                    puzzleFoiAtivado = false;
                    init_puzzle(player.faseAtual);
                }
            }
            if (fragmentoFoiAtivado) {
                draw_fragmento(fragmentoObrigatorioAtual.fase);
            }
            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            // DrawText(TextFormat("Camera X: %.2f", camera.target.x), 10, 50, 20, WHITE);
            // DrawText("SINT-7", 10, 10, 20, WHITE);
            
        EndDrawing();
    }

    UnloadGraphics();
    free_player_resources();
    CloseWindow();
    return 0;
}
