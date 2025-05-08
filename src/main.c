#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "graphics.h"
#include <stdbool.h>

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

int main() {
    InitWindow(900, 512, "SINT-7");
    SetTargetFPS(60);
    
    init_player();
    InitGraphics();
    InitCamera();

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
        if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

        update_player();
        UpdateCameraMove(player.position.x);

        camera.target = (Vector2){ player.position.x + player.width/2, SCREEN_HEIGHT / 2.0f };

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                DrawBackground();  // <<--- DESENHA OS SETORES
                draw_player();     // <<--- DESENHA O PLAYER
            EndMode2D();

            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            DrawText(TextFormat("Camera X: %.2f", camera.target.x), 10, 50, 20, WHITE);
            DrawText("HUD ou UI fora da camera", 10, 10, 20, WHITE);
        EndDrawing();
    }

    UnloadGraphics();
    free_player_resources();
    CloseWindow();
    return 0;
}
