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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SINT-7");
    SetTargetFPS(60);
    
    init_player();
    InitGraphics();
    InitCamera();
    init_fase();

    char interacao = '\0';  // variável global para guardar tipo de interação
    
    while (!WindowShouldClose()) {
        // Movimento e atualização da câmera
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
        if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

        update_player();
        UpdateCameraPosition();
        UpdateCameraMove();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                DrawBackground();
                draw_player();
                draw_fragmento_trigger();
                interacao = check_colisoes(); // interação é detectada aqui, mas desenhada depois
            EndMode2D();

            // 🔹 Desenhar UI ou elementos estáticos (não afetados pela câmera)
            if(interacao == 'p') draw_puzzle(puzzleAtual.fase);
            else if(interacao == 'f') draw_fragmento(fragmentoObrigatorioAtual.fase);

            // Interface de debug ou HUD
            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);

        EndDrawing();
    }

    UnloadGraphics();
    free_player_resources();
    CloseWindow();
    return 0;
}

