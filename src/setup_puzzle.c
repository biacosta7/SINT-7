#include "raylib.h" 
#include "config.h"
#include "fragmentos.h"
#include "puzzles.h"
#include "setup_puzzle.h"
#include "player.h"
#include "graphics.h"
#include "decode_puzzle.h"

bool puzzleFoiAtivado = false;
int countPuzzleCarregado = 0;
bool blocoFoiAtivado = false;


Puzzle puzzles[NUM_FRAGMENTOS] = { 
    { false, NULL, NULL, 1, 1095, 290 },
    { false, NULL, NULL, 2, 3800, 310 },
    { false, NULL, NULL, 3, 0, 800 },
    { false, NULL, NULL, 4, 10470, 300 },
    // TO-DO: adicionar o resto dos puzzles e texture
};

Bloco blocos[4] = { 
    { 0, false, 2230, 330 }, // Conexao
    { 1, false, 6700, 280 },   // ativacao enigma 3700
    { 2, false, 7452, 300 },  //conflito
    { 3, false, 9030, 300 }, // desconexao 9030
};


void init_puzzle(int fase){
    puzzleAtual = puzzles[fase-1];

    if(fase == 1 || fase == 4){
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
    
    //DrawRectangle(puzzleHitbox.x, puzzleHitbox.y, 72, 130, PURPLE);
    
    if (CheckCollisionRecs(playerHitbox, puzzleHitbox)) {
        DrawTextoInteracaoComFundo(puzzleAtual.x - 50, puzzleAtual.y - 30);
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

bool check_colisao_puzzle4(Rectangle playerHitbox){
    // Hitbox do puzzle
    Rectangle puzzleHitbox = {
        puzzles[3].x,
        puzzles[3].y,
        72, //width
        130 //height
    };
    
    DrawRectangle(puzzleHitbox.x, puzzleHitbox.y, 72, 130, YELLOW);
    
    if (CheckCollisionRecs(playerHitbox, puzzleHitbox)) {
        DrawTextoInteracaoComFundo(puzzles[3].x - 50, puzzles[3].y - 30);
        if (IsKeyPressed(KEY_I)) {
            if (!puzzles[3].foiSolucionado) {
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
    } else if(puzzleAtual.fase == 4){
        float scale = 18.0f;
        float textureWidth = 42 * scale; // ou puzzleAtual.texture.width * scale
        Vector2 position = {
            SCREEN_WIDTH / 2.0f - textureWidth / 2.0f,
            20
        };
	    DrawTextureEx(puzzleAtual.texture, position, 0.0f, scale, WHITE);
        alternar_estado_fundo_escuro(true);
        puzzle_4();
	}
}

void free_puzzle_resources() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
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

void carregarBlocos(){
    for(int i=0; i<4; i++){
        char path[64];
        sprintf(path, "assets/puzzles/blocos/%d.png", i);
        blocos[i].texture = LoadTexture(path);
    }
}

void drawBlocos(){
    for(int i=0; i<4; i++){
        Vector2 position = {blocos[i].x, blocos[i].y};
        float scale = 3.0f;
        DrawTextureEx(blocos[i].texture, position, 0.0f, scale, WHITE);
    }
}

void freeBlocos(){
    for(int i=0; i<4; i++){
        UnloadTexture(blocos[i].texture);
    }
}

bool checar_colisao_blocos(Rectangle playerHitbox){
    for(int i=0; i<4; i++){
        Rectangle blocoHitbox = {
            blocos[i].x,
            blocos[i].y,
            72, //width
            130 //height
        };
        
        if (CheckCollisionRecs(playerHitbox, blocoHitbox)) {
            DrawText("(C) para coletar", blocos[i].x - 50, blocos[i].y - 30, 20, GREEN);
            blocoAtual = blocos[i];

            if (IsKeyPressed(KEY_C)) {
                blocoFoiAtivado = true;
                
                printf("APERTOU C!!!!!\n");
                
                return true;
            }
        }
    }    
    return false;
}
