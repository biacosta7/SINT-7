#include "puzzles.h"
#include "setup_puzzle.h"
#include "player.h"
#include "graphics.h"
#include <math.h>

static bool moduloAnaliticoProximo = false;
static bool moduloEmpaticoProximo = false;
static bool mostrandoMensagem = true;
static int fadeAlpha = 0;
static bool decisaoFeita = false;
bool puzzle3Finalizado = false;

// Lógica dos Puzzles
void puzzle_1() {
    printf("\npPuzzle 1: layer.x: %f | player.y: %f\n", player.x, player.y);
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


#define MAX_CONNECTIONS 8

typedef struct {
    KeyboardKey key;
    int value;      // 0 ou 1
    int ledIndex;   // 0 = LED1, 1 = LED2, etc.
    bool active;
} Connection;

void puzzle_2() {
    static bool initialized = false;
    static Texture2D ledOn, ledOff, ledNeutro, board;
    static bool connections[MAX_CONNECTIONS] = {false};
    static bool resolved = false;

    int offsetX = 70;

    if (!initialized) {
        ledOn = LoadTexture("assets/puzzles/led_on.png");
        ledOff = LoadTexture("assets/puzzles/led_off.png");
        ledNeutro = LoadTexture("assets/puzzles/led_neutro.png");
        board = LoadTexture("assets/puzzles/circuito.png");
        initialized = true;
    }

    Connection connectionDefs[MAX_CONNECTIONS] = {
        {KEY_A, 0, 1},
        {KEY_B, 0, 3},
        {KEY_C, 0, 2},
        {KEY_D, 0, 0},
        {KEY_E, 1, 0},
        {KEY_F, 1, 2},
        {KEY_G, 1, 1},
        {KEY_H, 1, 3}
    };

    // Alternar conexões
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (IsKeyPressed(connectionDefs[i].key)) {
            connections[i] = !connections[i];
        }
    }

    int leds[4] = {-1, -1, -1, -1};
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i]) {
            leds[connectionDefs[i].ledIndex] = connectionDefs[i].value;
        }
    }

    resolved = (leds[0] == 0 && leds[1] == 1 && leds[2] == 0 && leds[3] == 1);
    if (resolved && !puzzleAtual.foiSolucionado) {
        puzzleAtual.foiSolucionado = true;
        if (!player.fasesDesbloqueadas[2]) {
            desbloquear_fase(2);
        }
    }

    DrawTexture(board, offsetX, 0, WHITE);

    Vector2 ledPositions[4] = {
        {620 + offsetX, 63}, {620 + offsetX, 143}, {620 + offsetX, 213}, {620 + offsetX, 287}
    };

    for (int i = 0; i < 4; i++) {
        Texture2D tex = (leds[i] == 1) ? ledOn : (leds[i] == 0 ? ledOff : ledNeutro);
        DrawTexture(tex, ledPositions[i].x, ledPositions[i].y, WHITE);
    }

    if (resolved) {
        DrawText("Validado!", 300 + offsetX, 430, 24, GREEN);
    } else {
        DrawText("Pressione A-H para conectar saídas", 220 + offsetX, 430, 20, GRAY);
    }
}

void verificar_posicao_player_puzzle3() {
    if (player.faseAtual == 3 && !puzzleAtual.foiSolucionado) {
        if (player.position.x >= 3880 && player.position.x <= 3930) {
            printf("analitico pertinho");
            moduloAnaliticoProximo = true;
        } else {
            moduloAnaliticoProximo = false;
        }
        if (player.position.x >= 5040 && player.position.x <= 5090) {
            moduloEmpaticoProximo = true;
        } else {
            moduloEmpaticoProximo = false;
        }
    }
}

void puzzle_3() {
    if (player.faseAtual == 3 && !puzzle3Finalizado) {
        if (mostrandoMensagem && !decisaoFeita) {
            if (fadeAlpha < 255) {
                fadeAlpha += 5;
            }
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.9f));
            if (fadeAlpha >= 255) {
                DrawText("Os recursos são limitados.", 80, 150, 30, WHITE);
                DrawText("Apenas um pode ser salvo.", 80, 190, 30, WHITE);
                DrawText("Um era razão pura — preciso, lógico.", 80, 250, 24, GRAY);
                DrawText("O outro... falhava. Mas fazia-nos sorrir.", 80, 280, 24, GRAY);
                DrawText("Pense bem.", 80, 330, 30, WHITE);
                DrawText("Pressione (X) para continuar.", 80, 390, 20, LIGHTGRAY);
                if (IsKeyPressed(KEY_X)) {
                    mostrandoMensagem = false;
                }
            }
            return;
        }

        if (!decisaoFeita) {
            if (moduloAnaliticoProximo) {
                DrawRectangle(100, 100, 500, 200, DARKGRAY);
                DrawText("Módulo Analítico", 120, 120, 25, SKYBLUE);
                DrawText("Desempenho: 99.9%", 120, 160, 20, WHITE);
                DrawText("Prioridade: Eficiência, cálculo e lógica.", 120, 190, 20, WHITE);
                DrawText("(E) para reativar", 120, 230, 20, GREEN);
                if (IsKeyPressed(KEY_E)) {
                    decisaoFeita = true;
                    puzzleAtual.foiSolucionado = true;
                }
            } else if (moduloEmpaticoProximo) {
                DrawRectangle(100, 100, 500, 200, DARKGRAY);
                DrawText("Módulo de Empatia", 120, 120, 25, PINK);
                DrawText("Desempenho: Instável", 120, 160, 20, WHITE);
                DrawText("Mas era... humano, acolhedor.", 120, 190, 20, WHITE);
                DrawText("(E) para reativar", 120, 230, 20, GREEN);
                if (IsKeyPressed(KEY_E)) {
                    decisaoFeita = true;
                    puzzleAtual.foiSolucionado = true;
                }
            }
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
            DrawText("Decisão tomada. Algo mudou em você.", 100, 300, 25, WHITE);
            DrawText("Pressione (X) para sair.", 100, 340, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_X)) {
                alternar_estado_fundo_escuro(false);
                puzzleFoiAtivado = false;
                puzzle3Finalizado = true;  // <- MARCA COMO ENCERRADO
                // Resetar variáveis para não recomeçar
                mostrandoMensagem = false;
                fadeAlpha = 0;
                decisaoFeita = false;
            }
        }
    }
}

void atualizar_puzzle3() {
    verificar_posicao_player_puzzle3();
    puzzle_3();
    
    if (player.faseAtual == 3 && !puzzleFoiAtivado && !puzzleAtual.foiSolucionado) {
        if (moduloAnaliticoProximo) {
            DrawRectangle(player.position.x - 100, player.position.y - 70, 200, 30, Fade(BLACK, 0.7f));
            DrawText("Módulo Analítico", player.position.x - 90, player.position.y - 65, 20, SKYBLUE);
        } else if (moduloEmpaticoProximo) {
            DrawRectangle(player.position.x - 100, player.position.y - 70, 200, 30, Fade(BLACK, 0.7f));
            DrawText("Módulo Empático", player.position.x - 90, player.position.y - 65, 20, PINK);
        }
    }
}