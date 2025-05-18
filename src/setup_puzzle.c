#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"
#include "puzzles.h"
#include "setup_puzzle.h"
#include "player.h"
#include "graphics.h"

bool puzzleFoiAtivado = false;
int countPuzzleCarregado = 0;

Puzzle puzzles[TOTAL_FRAGMENTOS_OBRIGATORIOS] = { 
    { false, NULL, NULL, 1, 1095, 290 },
    { false, NULL, NULL, 2, 2900, 290 },
    { false, NULL, NULL, 3, 4700, 300 },
    { false, NULL, NULL, 4, 0, 0 },
    // TO-DO: adicionar o resto dos puzzles e texture
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
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
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
    else if(puzzleAtual.fase == 3){
        alternar_estado_fundo_escuro(true);
        puzzle_3();
    }
}

void free_puzzle_resources() {
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        UnloadTexture(puzzles[i].texture);
    }
}
void verifica_puzzle_interativo() {
    if (player.faseAtual == 3 && !puzzleFoiAtivado) {
        if (player.position.x >= 3850 && player.position.x <= 3950) {
            DrawText("Módulo Analítico - pressione [E] para ativar", 100, 100, 20, WHITE);
            if (IsKeyPressed(KEY_E)) {
                init_puzzle(3);
                puzzleFoiAtivado = true;
            }
        }

        // Verifica se o jogador está perto do módulo de Empatia
        if (player.position.x >= 5010 && player.position.x <= 5110) {
            DrawText("Módulo de Empatia - pressione [E] para ativar", 100, 130, 20, WHITE);
            if (IsKeyPressed(KEY_E)) {
                init_puzzle(3);
                puzzleFoiAtivado = true;
            }
        }
    }
}
