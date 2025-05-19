#include "../include/menu.h"
#include "player.h"
#include <stdio.h>

#define QUANT_BOTOES 2

static EstadoMenu estadoAtual = MENU_PRINCIPAL;
static Texture2D backgroundTexture;
static Texture2D logoTexture;
static Botao botoes[QUANT_BOTOES];

// Variáveis para a tela de lore
static TelaLore telaLore = {0};

// Texto da história
static const char* historiaSINT7 =
"SINT-7\n\n"
"Em um laboratorio abandonado da Corporacao Neurodyne,\n"
"repousam os restos de SINT-7, uma IA descontinuada.\n\n"
"Voce acorda sem memoria, apenas com um numero de serie\n"
"e a certeza de que algo deu errado.\n\n"
"Explore o complexo, recupere fragmentos de memoria e\n"
"reconstrua sua identidade.\n\n"
"Suas decisoes afetarao diretamente o destino de SINT-7\n"
"e o futuro da pesquisa em inteligencia artificial.\n\n"
"Descubra a verdade... mas cuidado com o que deseja saber.";

void init_menu() {
    backgroundTexture = LoadTexture("assets/inicio.jpg");

    int btnWidth = 200;
    int btnHeight = 50;
    int startY = 300;
    int spacing = 20;

    const char* textos[] = {"Jogar", "Sair"};
    int numBotoes = 2;

    for (int i = 0; i < numBotoes; i++) {
        botoes[i] = (Botao){
            .rect = {
                .x = (GetScreenWidth() - btnWidth) / 2,
                .y = startY + i * (btnHeight + spacing),
                .width = btnWidth,
                .height = btnHeight
            },
            .texto = textos[i],
            .hover = false
        };
    }

    telaLore = (TelaLore){
        .background = LoadTexture("assets/bg-historia.png"),
        .textoHistoria = historiaSINT7,
        .scrollOffset = 0,
        .scrollSpeed = 35.0f,
        .podePular = false,
        .tempoExibicao = 0,
    };
}

void DrawTextWithCustomSpacing(const char *text, int posX, int posY, int fontSize, int charSpacing, int lineSpacing, Color color) {
    int x = posX;
    int y = posY;
    
    while (*text) {
        if (*text == '\n') {
            // Quebra de linha: reseta x e avança y considerando o tamanho da fonte e o espaçamento entre linhas
            x = posX;
            y += fontSize + lineSpacing;
        } else {
            // Converte o caractere atual em string (necessário para DrawText)
            char s[2] = { *text, '\0' };
            DrawText(s, x, y, fontSize, color);
            // Obtém a largura do caractere para ajustar o espaçamento horizontal
            int charWidth = MeasureText(s, fontSize);
            x += charWidth + charSpacing;
        }
        text++;
    }
}

void update_menu() {
    Vector2 mousePos = GetMousePosition();

    if (estadoAtual == MENU_PRINCIPAL) {
        for (int i = 0; i < QUANT_BOTOES; i++) {
            botoes[i].hover = CheckCollisionPointRec(mousePos, botoes[i].rect);

            if (botoes[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                switch (i) {
                    case 0:
                        estadoAtual = MENU_LORE;
                        telaLore.scrollOffset = 0;
                        telaLore.podePular = false;
                        telaLore.tempoExibicao = 0;
                        break;
                    case 1:
                        CloseWindow();
                        break;
                }
            }
        }
    } else if (estadoAtual == MENU_LORE) {
        telaLore.scrollOffset += GetFrameTime() * telaLore.scrollSpeed;
        telaLore.tempoExibicao += GetFrameTime();

        if (telaLore.tempoExibicao > 2.0f) {
            telaLore.podePular = true;
        }

        float textoAlturaTotal = 400.0f; // Valor aproximado para o tamanho total da história
        bool textoCompleto = telaLore.scrollOffset > (textoAlturaTotal + GetScreenHeight());

        if ((telaLore.podePular && (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) || textoCompleto) {
            estadoAtual = MENU_JOGANDO;
            init_player();
        }
    } else if (estadoAtual == MENU_JOGANDO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_PAUSADO;
    } else if (estadoAtual == MENU_PAUSADO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_JOGANDO;
    }
}

void draw_menu() {
    switch (estadoAtual) {
        case MENU_PRINCIPAL:
            DrawTexturePro(backgroundTexture,
                (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0}, 0, WHITE
            );

            for (int i = 0; i < QUANT_BOTOES; i++) {
                Color btnColor = botoes[i].hover ? SKYBLUE : (Color){200, 200, 200, 200};
                DrawRectangleRounded(botoes[i].rect, 0.3f, 10, btnColor);

                int textWidth = MeasureText(botoes[i].texto, 20);
                DrawText(botoes[i].texto,
                    botoes[i].rect.x + (botoes[i].rect.width - textWidth) / 2,
                    botoes[i].rect.y + 15,
                    20, DARKBLUE);
            }

            DrawText("© 2025 SINT-7 Game", 20, GetScreenHeight() - 30, 15, WHITE);
            break;

        case MENU_LORE:
            DrawTexturePro(telaLore.background,
                (Rectangle){0, 0, telaLore.background.width, telaLore.background.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0}, 0, (Color){255, 255, 255, 200}
            );

            // Define os parâmetros para espaçamentos
            int fontSize = 24;
            int charSpacing = 3;  // ajuste conforme sua necessidade
            int lineSpacing = 10; // ajuste conforme sua necessidade

            // A posição inicial leva em conta a rolagem
            int posX = 50;
            int posY = GetScreenHeight() - (int)telaLore.scrollOffset;

            DrawTextWithCustomSpacing(telaLore.textoHistoria, posX, posY, fontSize, charSpacing, lineSpacing, WHITE);

            // Instrução para pular
            if (telaLore.podePular) {
                Color cianoNeon = (Color){0, 217, 224, 255};
                const char* skipText = "Pressione ENTER ou clique para continuar";
                int fontSizeSkip = 20;
                int padding = 6;

                int textWidth = MeasureText(skipText, fontSizeSkip);
                int textHeight = fontSizeSkip;

                int posXSkip = (GetScreenWidth() - textWidth) / 2;
                int posYSkip = GetScreenHeight() - 50;

                DrawRectangle(posXSkip - padding, posYSkip - padding,
                            textWidth + padding * 2,
                            textHeight + padding * 2,
                            (Color){0, 0, 0, 200});
                DrawText(skipText, posXSkip, posYSkip, fontSizeSkip, cianoNeon);
            }
            break;


        case MENU_PAUSADO:
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
            const char* texto = "PAUSADO";
            int textWidth = MeasureText(texto, 40);
            DrawText(texto,
                (GetScreenWidth() - textWidth) / 2,
                GetScreenHeight() / 2 - 50,
                40, WHITE);

            const char* instrucao = "Pressione ESC para continuar";
            textWidth = MeasureText(instrucao, 20);
            DrawText(instrucao,
                (GetScreenWidth() - textWidth) / 2,
                GetScreenHeight() / 2 + 20,
                20, WHITE);
            break;

        default:
            break;
    }
}

void unload_menu_textures() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(telaLore.background);
}

EstadoMenu get_estado_menu() {
    return estadoAtual;
}

void set_estado_menu(EstadoMenu estado) {
    estadoAtual = estado;
}
