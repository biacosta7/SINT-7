#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "graphics.h"
#include <stdbool.h>

extern Camera2D camera; 

int main() {
    
    InitWindow(900, 512, "SINT-7");
    SetTargetFPS(60);
    
    init_player();
    InitGraphics();
    InitCamera();

    Vector2 offset = { 0,0 };

    Image bgImage = LoadImage("assets/setores/setor1.png");
    ImageResize(&bgImage, 900, bgImage.height); // Só ajusta altura
    Texture2D background = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);
    

    while (!WindowShouldClose()) {
        // Atualiza movimentação do jogador
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
        if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

        offset.x += GetFrameTime() * 100;
        offset.y -= GetFrameTime() * 100;

        camera.target = (Vector2){ player.position.x + player.width/2, player.position.y + player.height/2 };

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                // Desenhe aqui o mundo, incluindo o player e o cenário
                
                //DrawTexture(background, 0, 0, BLACK);
                DrawRectangleV(player.position, (Vector2){player.width, player.height}, BLUE);

                update_player();
                draw_player();

                
            EndMode2D();
            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            DrawText(TextFormat("Camera X: %.2f", camera.target.x), 10, 50, 20, WHITE);


            DrawText("HUD ou UI fora da camera", 10, 10, 20, WHITE);
        
        EndDrawing();


          // o draw_player já deve descontar cameraX

        //EndDrawing();
    }

    UnloadGraphics();
    free_player_resources();
    CloseWindow();
    return 0;
}
