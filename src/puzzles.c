#include "puzzles.h"
#include "setup_puzzle.h"
#include "player.h"

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

