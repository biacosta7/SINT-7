#include "raylib.h"
#include "game.h"
#include "player.h"
#include "graphics.h"
#include "puzzles.h"
#include "utils.h"
#include <stdbool.h>

int main(){
    InitWindow(800, 600, "SINT-7");
    SetTargetFPS(60);

    init_player();

    while (!WindowShouldClose()) {
        update_player();


        BeginDrawing();
        ClearBackground(BLACK);
        DrawBackground(player.position);
        draw_player();
        EndDrawing();
    }

    free_player_resources();
    UnloadGraphics();  
    CloseWindow();
    return 0;
}