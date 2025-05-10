// #include "raylib.h"
// #include "game.h"
// #include "player.h"
// #include "graphics.h"
// #include "puzzles.h"
// #include "utils.h"
// #include <stdbool.h>

// int main(){
//     InitWindow(900, 515, "SINT-7");
//     SetTargetFPS(60);

//     init_player();
//     InitGraphics();
//     Image bgImage = LoadImage("assets/setores/setor1.png");
//     ImageResize(&bgImage, 900, bgImage.height); // Só ajusta altura
//     Texture2D background = LoadTextureFromImage(bgImage);
//     UnloadImage(bgImage);

//     float cameraX = 0.0f;
//     const float scrollSpeed = 2.0f;  // ajuste como quiser

//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(BLACK);

//         if (IsKeyDown(KEY_RIGHT)) cameraX += scrollSpeed;
//         if (IsKeyDown(KEY_LEFT))  cameraX -= scrollSpeed;

//         if (cameraX < 0) cameraX = 0;
//         if (cameraX > background.width - GetScreenWidth())
//             cameraX = background.width - GetScreenWidth();

//         Rectangle source = { cameraX, 0, GetScreenWidth(), background.height };
//         Rectangle dest = { 0, 0, GetScreenWidth(), GetScreenHeight() };

//         DrawTexturePro(background, source, dest, (Vector2){0, 0}, 0.0f, WHITE);

//         //DrawBackground();
//         draw_player();
//         update_player();
//         EndDrawing();
//     }

//     UnloadGraphics();
//     UnloadImage(bgImage);
    
//     free_player_resources();
//     CloseWindow();
//     return 0;
// } 


// fragmentos
// int empatia = 0, obediencia = 0, autonomia = 0;

// for (int i = 0; i < TOTAL_FRAGMENTOS; i++) {
//     if (fragmentos[i].desbloqueado) {
//         switch (fragmentos[i].ideia) {
//             case EMPATIA: empatia++; break;
//             case OBEDIENCIA: obediencia++; break;
//             case AUTONOMIA: autonomia++; break;
//         }
//     }
// }