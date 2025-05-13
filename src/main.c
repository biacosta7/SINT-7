#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "puzzles.h"
#include "graphics.h"
#include "fase.h"
#include <stdbool.h>
#include <stdio.h>

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

int main() {
    NodeFragmento *fragmentosColetados = NULL;
    InitWindow(900, 512, "SINT-7");
    SetTargetFPS(60);
    
    init_player();
    InitGraphics();
    InitCamera();
    init_fase();

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
        if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

        update_player(); // Atualiza jogador
        UpdateCameraPosition(); // Atualiza camera.target.x se necessário
        UpdateCameraMove(); // Atualiza cameraX baseado na nova camera

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                DrawBackground();  // DESENHA OS SETORES
                draw_player();     // DESENHA O PLAYER
                draw_fragmento_trigger(); // DESENHA O TRIGGER FRAGMENTO
                char interacao = check_colisoes(); // Chamar apenas uma vez
                
            EndMode2D();
            
            if(interacao == 'p') draw_puzzle(puzzleAtual.fase);
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
