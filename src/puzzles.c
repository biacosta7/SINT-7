#include "puzzles.h"
#include "player.h"
#include <string.h>

Texture2D fragmentoTexture, bgfragmentoTexture;
FragmentoMemoria fragmentoOpcionalAtual;
NodeFragmento *fragmentosColetados = NULL;
bool fragmentoFoiAtivado = false;

FragmentoMemoria fragmentosOpcionais[TOTAL_FRAGMENTOS_OPCIONAIS] = {
    { true, false, "\"\"", 1,  NULL  },
    { true, false, "\"\"", 2,  NULL  },
    { false, false, "\"\"", 3, NULL },
    { false, false, "\"\"", 4, NULL },
    // TO-DO: adicionar o resto dos enigmas
};
void update_fragmento_opcional() {
    for (int i = 0; i < TOTAL_FRAGMENTOS_OPCIONAIS; i++) {
        if (fragmentosOpcionais[i].fase == player.faseAtual) {
            fragmentoOpcionalAtual = fragmentosOpcionais[i];
            char path[64];
            sprintf(path, "assets/fragmentos/background-frag/%03d.png", player.faseAtual);
            fragmentosOpcionais[i].texture = LoadTexture(path);

            char trigger_path[64];
            sprintf(trigger_path, "assets/fragmentos/trigger-frag/%03d.png", player.faseAtual);
            fragmentosOpcionais[i].trigger = LoadTexture(trigger_path);

            break;
        }
    }
}

FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS] = {
    { true, false, "\"O padrão era sempre primo. Ela dizia: 2, 3, 5...\"", 1, ENIGMA, 550, 350 },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA, 2030, 330 },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA },
    { false, false, "\"Eu nasci do silêncio. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

Puzzle puzzles[TOTAL_FRAGMENTOS_OBRIGATORIOS] = { 
    { false, NULL, NULL, 1, 1095, 290 },
    { false, NULL, NULL, 2, 2900, 350 },
    { false, NULL, NULL, 3, 0, 0 },
    { false, NULL, NULL, 4, 0, 0 },
    // TO-DO: adicionar o resto dos puzzles e texture
};

//TO-DO: pegar onde a camera tá e colocar o conteudo do fragmento no top centro

void init_fragmento(int fase){
    fragmentoTexture = LoadTexture("assets/fragmentos/Random Device 2.png");
    bgfragmentoTexture = LoadTexture("assets/fragmentos/background-frag.png");
    
    fragmentoObrigatorioAtual = fragmentosObrigatorios[fase-1];
}

void update_fragmento() {
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        if (fragmentosObrigatorios[i].fase == player.faseAtual) {
            fragmentoObrigatorioAtual = fragmentosObrigatorios[i];

            char path[64];
            sprintf(path, "assets/fragmentos/background-frag/%03d.png", player.faseAtual);
            fragmentosObrigatorios[i].texture = LoadTexture(path);

            char trigger_path[64];
            sprintf(trigger_path, "assets/fragmentos/trigger-frag/%03d.png", player.faseAtual);
            fragmentosObrigatorios[i].trigger = LoadTexture(trigger_path);

            break;
        }
    }
}

void update_puzzle(){
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        if (puzzles[i].fase == player.faseAtual) {
            puzzleAtual = puzzles[i];
            break;
        }
    }
}


void init_puzzle(int fase){
    puzzleAtual = puzzles[fase-1];
    if(fase == 1 || fase == 2){
        puzzleAtual.texture = LoadTexture("assets/puzzles/terminal.png");
    }
    
}

void adicionar_fragmento(FragmentoMemoria novoFragmento) {
    NodeFragmento *novo = (NodeFragmento *)malloc(sizeof(NodeFragmento));
    if (!novo) return; // Falha de alocação

    novo->fragmento = novoFragmento;
    novo->next = fragmentosColetados;
    fragmentosColetados = novo;
}


void printar_fragmentos() {
    NodeFragmento *atual = fragmentosColetados;
    int i = 1;
    
    if (!atual) {
        printf("Nenhum fragmento coletado.\n");
        return;
    }

    printf("Fragmentos coletados:\n");
    while (atual != NULL) {
        printf("Fragmento %d:\n", i++);
        printf("  Conteúdo: %s\n", atual->fragmento.conteudo);
        printf("  Fase: %d\n", atual->fragmento.fase);
        printf("  Obrigatório: %s\n", atual->fragmento.ehObrigatorio ? "Sim" : "Não");
        printf("  Foi coletado: %s\n", atual->fragmento.foiColetado ? "Sim" : "Não");
        printf("  Posição: (%.2f, %.2f)\n", atual->fragmento.x, atual->fragmento.y);
        printf("----------------------------------\n");

        atual = atual->next;
    }
}

bool fragmento_ja_coletado(const char *conteudo) {
    NodeFragmento *atual = fragmentosColetados;
    while (atual != NULL) {
        if (strcmp(atual->fragmento.conteudo, conteudo) == 0) {
            return true;
        }
        atual = atual->next;
    }
    return false;
}


// Colisões
bool check_colisao_fragmento(Rectangle playerHitbox){

    // HITBOX DO FRAGMENTO
    Rectangle fragmentoHitbox = {
        fragmentoObrigatorioAtual.x,
        fragmentoObrigatorioAtual.y,
        32, //width
        120 //height
    };

    //para ver onde ta a caixa de colisao:
    DrawRectangle(fragmentoHitbox.x, fragmentoHitbox.y, fragmentoHitbox.width, fragmentoHitbox.height, YELLOW);
    // DrawRectangle(playerHitbox.x, playerHitbox.y, playerHitbox.width, playerHitbox.height, GREEN);

    // colisão com fragmento
    if (CheckCollisionRecs(playerHitbox, fragmentoHitbox)) {
        DrawText("(F) para interagir", fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30, 20, GREEN);
        
        if (IsKeyPressed(KEY_F)) {
            if (!fragmento_ja_coletado(fragmentoObrigatorioAtual.conteudo)) {
                fragmentoObrigatorioAtual.foiColetado = true;
                adicionar_fragmento(fragmentoObrigatorioAtual);
                printar_fragmentos();
            }
            fragmentoFoiAtivado = !fragmentoFoiAtivado;
            return true;
        }
    }
    return false;
}

bool check_colisao_puzzle(Rectangle playerHitbox){
    // Hitbox do puzzle
    Rectangle puzzleHitbox = {
        puzzleAtual.x,
        puzzleAtual.y,
        72, //width
        130 //height
    };
    
    DrawRectangle(puzzleHitbox.x, puzzleHitbox.y, puzzleHitbox.width, puzzleHitbox.height, PURPLE);

    if (CheckCollisionRecs(playerHitbox, puzzleHitbox)) {
        DrawText("(F) para interagir", puzzleAtual.x - 50, puzzleAtual.y - 30, 20, GREEN);
        if (IsKeyDown(KEY_F)){ // ERRO: texto não mantem
            
            if(!puzzleAtual.foiSolucionado){
                puzzleAtual.foiSolucionado = true;
            }

            return true;
        }
    }
    return false;
}

char check_colisoes(){
    // HITBOX DO PLAYER
    Rectangle playerHitbox = {
        player.position.x,
        player.position.y,
        16 * 5,  // mesmo scale do draw_player
        16 * 5
    };
    bool fragmento = check_colisao_fragmento(playerHitbox);
    bool puzzle = check_colisao_puzzle(playerHitbox);

    if (fragmento) return 'f';
    else if (puzzle) return 'p';
    
    return 'z';
}


// Draw Puzzle e Fragmento
void draw_puzzle(int puzzle){
    float scale = 18.0f;
    float textureWidth = 42 * scale; // ou puzzleAtual.texture.width * scale
    Vector2 position = {
        SCREEN_WIDTH / 2.0f - textureWidth / 2.0f,
        20
    };

    if(puzzleAtual.fase == 1 || puzzleAtual.fase == 2) { //fases que usam terminal
        DrawTextureEx(puzzleAtual.texture, position, 0.0f, scale, WHITE);
        if (puzzleAtual.fase == 1) puzzle_1();
    }
}

void draw_fragmento(int fragmento){
    float scale = 1.0f;
    float textureWidth = 513 * scale; 

    Vector2 position = {
        SCREEN_WIDTH / 2 - textureWidth / 2, 
        20
    };
    DrawTextureEx(fragmentoObrigatorioAtual.texture, position, 0.0f, scale, WHITE);

}

void draw_fragmento_trigger(){
    Vector2 position = {fragmentoObrigatorioAtual.x, fragmentoObrigatorioAtual.y};
    float scale = 3.0f;
    DrawTextureEx(fragmentoObrigatorioAtual.trigger, position, 0.0f, scale, WHITE);
}

// Lógica dos Puzzles
void puzzle_1() {
    static int input[4] = { -1, -1, -1, -1 };
    static int inputIndex = 0;
    static bool success = false;
    static bool error = false;

    const int respostaCorreta[4] = { 2, 3, 5, 7 };

    int buttonSize = 50;
    int padding = 10;
    int startX = (SCREEN_WIDTH / 2 - 21) - 130; //42 (tam do terminal) / 2 = 21
    int startY = 150;

    // Desenha os botões de 0 a 9
    for (int i = 0; i < 10; i++) {
        int row = i / 5;
        int col = i % 5;

        Rectangle button = {
            startX + col * (buttonSize + padding),
            startY + row * (buttonSize + padding),
            buttonSize,
            buttonSize
        };

        DrawRectangleRec(button, LIGHTGRAY);
        DrawText(TextFormat("%d", i),
                 button.x + buttonSize / 2 - 5,
                 button.y + buttonSize / 2 - 10,
                 20, BLACK);

        // Clique no botão
        if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (inputIndex < 4) {
                input[inputIndex++] = i;
                success = false;
                error = false;

                if (inputIndex == 4) {
                    // Verifica sequência
                    bool match = true;
                    for (int j = 0; j < 4; j++) {
                        if (input[j] != respostaCorreta[j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        success = true;
                        puzzleAtual.foiSolucionado = true;
                        // Desbloqueia a fase 2 se ainda não estiver desbloqueada
                        if (!player.fasesDesbloqueadas[1]) {
                            desbloquear_fase(1);
                        }
                    } else {
                        error = true;
                    }
                }
            }
        }
    }

    // Mostra entrada atual
    DrawText(">", startX, startY + 130, 20, GREEN);
    for (int i = 0; i < inputIndex; i++) {
        DrawText(TextFormat("%d", input[i]), startX + 20 + i * 20, startY + 130, 20, WHITE);
    }

    // Mensagem de resultado
    if (success) {
        DrawText("Aprovado.", startX, startY + 170, 20, GREEN);
    } else if (error) {
        DrawText("Acesso negado.", startX, startY + 170, 20, RED);
    }

    // Botão de reset
    Rectangle resetBtn = { startX  + 300, startY + 215, 100, 30 };
    DrawRectangleRec(resetBtn, DARKGRAY);
    DrawText("Resetar", resetBtn.x + 10, resetBtn.y + 5, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), resetBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        inputIndex = 0;
        for (int i = 0; i < 4; i++) input[i] = -1;
        success = false;
        error = false;
    }
}

void free_fragmento_resources() {
    NodeFragmento *atual = fragmentosColetados;
    while (atual != NULL) {
        NodeFragmento *temp = atual;
        atual = atual->next;
        free(temp);
    }
    fragmentosColetados = NULL;

    UnloadTexture(fragmentoTexture);
    UnloadTexture(bgfragmentoTexture);

    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        UnloadTexture(fragmentosObrigatorios[i].texture);
        UnloadTexture(fragmentosObrigatorios[i].trigger);
    }
}
void free_puzzle_resources() {
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        UnloadTexture(puzzles[i].texture);
    }
}