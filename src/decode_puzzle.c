// decode_puzzle_raylib.c
// Puzzle: Enigma de Decodificação (integrado ao layout de puzzle_2)

#include "decode_puzzle.h"
#include <stdio.h>
#include <string.h>

void UpdateTypewriterText(TypewriterText *tw, float delta) {
    if (tw->completed) return;
    tw->timer += delta;
    if (tw->timer >= tw->delayPerChar) {
        tw->timer = 0;
        tw->visibleChars++;
        if ((int)strlen(tw->text) <= tw->visibleChars) tw->completed = true;
    }
}

void DrawTypewriterText(TypewriterText *tw, int x, int y, int size, Color color) {
    DrawText(TextSubtext(tw->text, 0, tw->visibleChars), x, y, size, color);
}

void puzzle_decode() {
    static bool initialized = false;
    static Texture2D row1[ROW_SIZE];
    static Texture2D row2[ROW_SIZE];
    static Texture2D progressSprites[SEQ_LENGTH];
    static Texture2D board;
    static int correctOrder[SEQ_LENGTH] = {0,1,2,3,4,5,6};

    static int currentIndex = 0;
    static int indexRow1 = 0;
    static int indexRow2 = 0;
    static float timer = TOTAL_TIME;
    static PuzzleState state = PS_Initializing;

    static TypewriterText msg1 = {"Rastreando servidor...", 0.05f, 0, 0, false};
    static TypewriterText msg2 = {"WARNING: Ameaça detectada", 0.05f, 0, 0, false};
    static TypewriterText msg3 = {"Necessita código de autenticação. Completar decodificação...", 0.03f, 0, 0, false};

    int offsetX = 140;
    int offsetXBorad = 70;
    int panelWidth = 600; // largura aproximada do puzzle

    if (!initialized) {
        board = LoadTexture("assets/puzzles/decode/pc.png");
        for (int i = 0; i < SEQ_LENGTH; i++) {
            char path1[64];
            sprintf(path1, "assets/puzzles/decode/seg/%d.png", i + 1);
            row1[i] = LoadTexture(path1);

            char path2[64];
            sprintf(path2, "assets/puzzles/decode/seg/%d.png", i + 8);
            row2[i] = LoadTexture(path2);

            char ppath[64];
            sprintf(ppath, "assets/puzzles/decode/progress/%d.png", i + 1);
            progressSprites[i] = LoadTexture(ppath);
        }
        initialized = true;
    }

    DrawTexture(board, offsetXBorad, 0, WHITE);

    float dt = GetFrameTime();

    switch (state) {
        case PS_Initializing:
            UpdateTypewriterText(&msg1, dt);
            DrawTypewriterText(&msg1, offsetX, 50, 20, RED);
            if (msg1.completed) {
                UpdateTypewriterText(&msg2, dt);
                DrawTypewriterText(&msg2, offsetX, 80, 20, RED);
            }
            if (msg2.completed) {
                UpdateTypewriterText(&msg3, dt);
                DrawTypewriterText(&msg3, offsetX, 110, 20, RED);
            }
            if (msg3.completed) {
                DrawText("Pressione ENTER para iniciar", offsetX, 140, 20, GRAY);
                if (IsKeyPressed(KEY_ENTER)) state = PS_Countdown;
            }
            break;

        case PS_Countdown:
            state = PS_DisplaySequence;
            break;

        case PS_DisplaySequence: {
            float scale = 1.3f;
            Vector2 pos = {
                offsetX + (panelWidth / 2) - (progressSprites[currentIndex].width * scale / 2),
                120
            };
            DrawTextureEx(progressSprites[currentIndex], pos, 0.0f, scale, WHITE);

            DrawText("Pressione ENTER para decodificar", pos.x + 20, pos.y + 140, 20, GRAY);
            if (IsKeyPressed(KEY_ENTER)) state = PS_Selecting;
        } break;


        case PS_Selecting: {
            timer -= dt;
            float pct = timer / TOTAL_TIME;

            // Aumentando o espaçamento no topo (de 20 para 40)
            DrawRectangle(offsetX, 40, panelWidth * pct, 20, RED);
            DrawRectangleLines(offsetX, 40, panelWidth, 20, WHITE);

            // Texto de progresso com mais espaço (de 50 para 70)
            char buf[64];
            sprintf(buf, "Progresso: %.0f%% concluído... Tempo restante: %.0fs", pct * 100, timer);
            DrawText(buf, offsetX, 70, 20, RED);

            // Sprite de progresso com mais espaço (de 100 para 120)
            float scale = 1.3f;
            Vector2 pos = {
                offsetX + (panelWidth / 2) - (progressSprites[currentIndex].width * scale / 2),
                120
            };
            DrawTextureEx(progressSprites[currentIndex], pos, 0.0f, scale, WHITE);

            int centerX = offsetX + panelWidth / 2;
            int spacing = 60;
            int half = 2;

            // Fileiras reposicionadas para acompanhar o novo topo
            int y1 = 250;
            int y2 = y1 + 50; 

            for (int i = -half; i <= half; i++) {
                int idx = (indexRow1 + i + ROW_SIZE) % ROW_SIZE;
                Color tint = (i == 0) ? WHITE : (Color){255, 255, 255, 128};
                DrawTexture(row1[idx], centerX + i * spacing - row1[idx].width / 2, y1, tint);
            }
            for (int i = -half; i <= half; i++) {
                int idx = (indexRow2 + i + ROW_SIZE) % ROW_SIZE;
                Color tint = (i == 0) ? WHITE : (Color){255, 255, 255, 128};
                DrawTexture(row2[idx], centerX + i * spacing - row2[idx].width / 2, y2, tint);
            }

            int text1Width = MeasureText("< A / D > fileira 1", 20);
            int text2Width = MeasureText("Enter = Confirmar", 20);
            int text3Width = MeasureText("< J / L > fileira 2", 20);
            int padding = 40;
            int infoY = y2 + 60;

            // x1 = início
            int x1 = offsetX;
            // x3 = após o primeiro texto + padding
            int x3 = x1 + text1Width + padding;
            // x2 = após o segundo texto + padding
            int x2 = x3 + text2Width + padding;

            DrawText("< A / D > fileira 1", x1, infoY, 20, GRAY);
            DrawText("Enter = Confirmar", x3, infoY, 20, WHITE);
            DrawText("< J / L > fileira 2", x2, infoY, 20, GRAY);

            if (IsKeyPressed(KEY_A)) indexRow1 = (indexRow1 - 1 + ROW_SIZE) % ROW_SIZE;
            if (IsKeyPressed(KEY_D)) indexRow1 = (indexRow1 + 1) % ROW_SIZE;
            if (IsKeyPressed(KEY_J)) indexRow2 = (indexRow2 - 1 + ROW_SIZE) % ROW_SIZE;
            if (IsKeyPressed(KEY_L)) indexRow2 = (indexRow2 + 1) % ROW_SIZE;

            if (IsKeyPressed(KEY_ENTER)) {
                if (indexRow1 == correctOrder[currentIndex] && indexRow2 == correctOrder[currentIndex]) {
                    currentIndex++;
                    if (currentIndex >= SEQ_LENGTH) state = PS_Success;
                } else {
                    state = PS_Failure;
                }
            }
            if (timer <= 0) state = PS_Failure;
        } break;

        case PS_Success:
            DrawText("Bloco de Ativação Liberado", offsetX + 100, 320, 24, GREEN);
            break;

        case PS_Failure:
            DrawText("Timeout. Reiniciando Sistema...", offsetX + 20, 320, 24, RED);
            if (IsKeyPressed(KEY_ENTER)) {
                timer = TOTAL_TIME;
                currentIndex = 0;
                indexRow1 = indexRow2 = 0;
                state = PS_Initializing;
                msg1.visibleChars = msg2.visibleChars = msg3.visibleChars = 0;
                msg1.completed = msg2.completed = msg3.completed = false;
            }
            break;
    }
}
