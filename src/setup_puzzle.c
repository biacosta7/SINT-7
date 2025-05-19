#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"
#include "puzzles.h"
#include "setup_puzzle.h"
#include "player.h"
#include "graphics.h"
//4990
bool puzzleFoiAtivado = false;
int countPuzzleCarregado = 0;

Puzzle puzzles[NUM_FRAGMENTOS] = { 
    { false, NULL, NULL, 1, 1095, 290 },
    { false, NULL, NULL, 2, 3800, 310 },
    { false, NULL, NULL, 3, 5140, 300 },
    { false, NULL, NULL, 4, 0, 0 },
    // TO-DO: adicionar o resto dos puzzles e texture
};

Bloco blocos[4] = { 
    { false, 5290, 300, NULL },
    { false, 0, 0, NULL },
    { false, 0, 0, NULL },
    { false, 0, 0, NULL } 
};

void init_puzzle(int fase){
    puzzleAtual = puzzles[fase-1];

    if(fase == 1){
        puzzleAtual.texture = LoadTexture("assets/puzzles/terminal.png");
    } else if (fase == 2) {
        puzzleAtual.texture = LoadTexture("assets/puzzles/circuito.png");
    }

    puzzleFoiAtivado = false; 
}

void update_puzzle(){
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        if (puzzles[i].fase == player.faseAtual && i >= countPuzzleCarregado) {
            puzzles[i] = puzzles[i]; // mantém sincronizado, mas não seta puzzleAtual ainda
            countPuzzleCarregado++;
            break;
        }
    }
}

bool check_colisao_puzzle(Rectangle playerHitbox){
    // Hitbox do puzzle
    Rectangle puzzleHitbox = {
        puzzleAtual.x,
        puzzleAtual.y,
        72, //width
        130 //height
    };
    
    //DrawRectangle(puzzleHitbox.x, puzzleHitbox.y, puzzleHitbox.width, puzzleHitbox.height, PURPLE);
    

    if (CheckCollisionRecs(playerHitbox, puzzleHitbox)) {
        DrawText("(I) para interagir", puzzleAtual.x - 50, puzzleAtual.y - 30, 20, GREEN);
        if (IsKeyPressed(KEY_I)) {
            if (!puzzleAtual.foiSolucionado) {
                init_puzzle(player.faseAtual); // <<< só agora inicializa o puzzle
            }
            puzzleFoiAtivado = true;
            return true;
        }
    }
    return false;
}

void draw_puzzle(int puzzle){
    if(puzzleAtual.fase == 1) { //fases que usam terminal
        float scale = 18.0f;
        float textureWidth = 42 * scale; // ou puzzleAtual.texture.width * scale
        Vector2 position = {
            SCREEN_WIDTH / 2.0f - textureWidth / 2.0f,
            20
        };
        DrawTextureEx(puzzleAtual.texture, position, 0.0f, scale, WHITE);
        alternar_estado_fundo_escuro(true);
        puzzle_1();
    } else if (puzzleAtual.fase == 2) {
        alternar_estado_fundo_escuro(true);
        puzzle_2();
    }

}

void free_puzzle_resources() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        UnloadTexture(puzzles[i].texture);
    }
}

void carregarBlocos(){
    for(int i=0; i<4; i++){
        char path[64];
        sprintf(path, "assets/fragmentos/background-frag/obrigatorios/%d.png", i);
        blocos[i].texture = LoadTexture(path);
    }
}

void drawBlocos(){
    for(int i=0; i<4; i++){
        Vector2 position = {blocos[i].x, blocos[i].y};
        float scale = 1.0f;
        DrawTextureEx(blocos[i].texture, position, 0.0f, scale, WHITE);
    }
}

void freeBlocos(){
    for(int i=0; i<4; i++){
        UnloadTexture(blocos[i].texture);
    }
}

// void checar_colisao_blocos(Rectangle playerHitbox){
//     // Hitbox do puzzle
//     Rectangle puzzleHitbox = {
//         5290,
//         300,
//         72, //width
//         130 //height
//     };
    
//     if (CheckCollisionRecs(playerHitbox, puzzleHitbox)) {
//         DrawText("(I) para interagir", 5290 - 50, 300 - 30, 20, GREEN);
//         if (IsKeyPressed(KEY_I)) {
//             puzzleFoiAtivado = true;
//             return true;
//         }
//     }
//     return false;
// }