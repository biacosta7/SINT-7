#include "puzzles.h"
#include "player.h"

Texture2D fragmentoTexture, bgfragmentoTexture;
NodeFragmento *fragmentosColetados = NULL;

FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS] = {
    { true, false, "\"O padrão era sempre primo.\nEla dizia: 2, 3, 5... o último era 7.\"", 1, ENIGMA, 550, 350 },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA },
    { false, false, "\"Eu nasci do silência. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

Puzzle puzzles[TOTAL_FRAGMENTOS_OBRIGATORIOS] = { 
    { false, NULL, NULL, 1, 1095, 290 },
    { false, NULL, NULL, 2, 0, 0 },
    { false, NULL, NULL, 3, 0, 0 },
    { false, NULL, NULL, 4, 0, 0 },
    // TO-DO: adicionar o resto dos puzzles e texture
};

//TO-DO: pegar onde a camera tá e colocar o conteudo do fragmento no top centro

void init_fragmento(int fase){
    fragmentoTexture = LoadTexture("assets/fragmentos/Random Device 2.png");
    bgfragmentoTexture = LoadTexture("assets/fragmentos/background-frag.png");
    
    fragmentoObrigatorioAtual = fragmentosObrigatorios[fase-1];
    char path[64];
    sprintf(path, "assets/fragmentos/background-frag/00%d.png", fase);
    fragmentoObrigatorioAtual.texture = LoadTexture(path);
    
}

void init_puzzle(int fase){
    puzzleAtual = puzzles[fase-1];
    if(fase == 1 || fase == 2){
        puzzleAtual.texture = LoadTexture("assets/puzzles/terminal.png");
    }
    
}

void draw_fragmento(){
    if (fragmentoObrigatorioAtual.fase == 1){
        Vector2 position = {fragmentoObrigatorioAtual.x, fragmentoObrigatorioAtual.y};
        float scale = 3.0f;
        DrawTextureEx(fragmentoTexture, position, 0.0f, scale, WHITE);
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

bool check_colisao_fragmento(Rectangle playerHitbox){

    // HITBOX DO FRAGMENTO
    Rectangle fragmentoHitbox = {
        fragmentoObrigatorioAtual.x,
        fragmentoObrigatorioAtual.y,
        32, //width
        60 //height
    };

    //para ver onde ta a caixa de colisao:
    // DrawRectangle(fragmentoHitbox.x, fragmentoHitbox.y, fragmentoHitbox.width, fragmentoHitbox.height, YELLOW);
    // DrawRectangle(playerHitbox.x, playerHitbox.y, playerHitbox.width, playerHitbox.height, GREEN);
    //DrawRectangle(puzzleHitbox.x, puzzleHitbox.y, puzzleHitbox.width, puzzleHitbox.height, PURPLE);

    // colisão com fragmento
    if (CheckCollisionRecs(playerHitbox, fragmentoHitbox)) {
        DrawText("(F) para interagir", fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30, 20, GREEN);


        int fonteTamanho = 20;
        int textoLargura = MeasureText(fragmentoObrigatorioAtual.conteudo, fonteTamanho);
        int tituloLargura = MeasureText("Fragmento de Memória Encontrado", fonteTamanho);


        if (IsKeyDown(KEY_F)){ // ERRO: texto não mantem
            Vector2 position = {SCREEN_WIDTH / 2 - 513 / 2, 20};
            float scale = 1.0f;
            DrawTextureEx(fragmentoObrigatorioAtual.texture, position, 0.0f, scale, WHITE);
            
            if(!fragmentoObrigatorioAtual.foiColetado){
                fragmentoObrigatorioAtual.foiColetado = true;
                adicionar_fragmento(fragmentoObrigatorioAtual);
                printar_fragmentos();
            }
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
    if (puzzle) return 'p';
    return 'z';
}


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
    puzzle_1();
}

// puzzles

void puzzle_1() {
    static int input[4] = { -1, -1, -1, -1 };
    static int inputIndex = 0;
    static bool success = false;
    static bool error = false;

    const int respostaCorreta[4] = { 2, 3, 5, 7 };

    int buttonSize = 50;
    int padding = 10;
    int startX = 100;
    int startY = 100;

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
                    if (match) success = true;
                    else error = true;
                }
            }
        }
    }

    // Mostra entrada atual
    DrawText("Entrada:", startX, startY + 130, 20, BLACK);
    for (int i = 0; i < inputIndex; i++) {
        DrawText(TextFormat("%d", input[i]), startX + 80 + i * 20, startY + 130, 20, DARKBLUE);
    }

    // Mensagem de resultado
    if (success) {
        DrawText("Correto!", startX, startY + 170, 20, GREEN);
    } else if (error) {
        DrawText("Incorreto!", startX, startY + 170, 20, RED);
    }

    // Botão de reset
    Rectangle resetBtn = { startX, startY + 210, 100, 30 };
    DrawRectangleRec(resetBtn, GRAY);
    DrawText("Resetar", resetBtn.x + 10, resetBtn.y + 5, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), resetBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        inputIndex = 0;
        for (int i = 0; i < 4; i++) input[i] = -1;
        success = false;
        error = false;
    }
}


void unload_fragmento() {
    UnloadTexture(fragmentoTexture);
    UnloadTexture(fragmentoObrigatorioAtual.texture);
}
