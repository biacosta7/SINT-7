#include "../include/menu.h"
#include "player.h"
#include <stdio.h>

#define QUANT_BOTOES 3

static EstadoMenu estadoAtual = MENU_PRINCIPAL;
static Texture2D backgroundTexture;
static Texture2D logoTexture;
static Botao botoes[QUANT_BOTOES];
static Font fonte;

// Variáveis para a tela de lore
static TelaLore telaLore = {0};

// Texto da história (adaptado do que você forneceu)
static const char* historiaSINT7 = 
"SINT-7\n\n"
"Em um laboratorio abandonado da Corporacao Neurodyne,\n"
"repousa os restos de SINT-7, uma IA descontinuada.\n\n"
"Voce acorda sem memoria, apenas com um numero de serie\n"
"e a certeza de que algo deu errado.\n\n"
"Explore o complexo, recupere fragmentos de memoria e\n"
"reconstrua sua identidade.\n\n"
"Suas decisoes afetarao diretamente o destino de SINT-7\n"
"e o futuro da pesquisa em inteligencia artificial.\n\n"
"Descubra a verdade... mas cuidado com o que deseja saber.";

void init_menu() {
    // Carrega texturas do menu principal
    backgroundTexture = LoadTexture("assets/inicio.jpg");
    fonte = LoadFontEx("assets/arguments/font.ttf", 32, 0, 250); // Ajuste o caminho se necessário

    // Configura botões do menu principal
    int btnWidth = 200;
    int btnHeight = 50;
    int startY = 300;
    int spacing = 20;

    const char* textos[] = {"Jogar", "Configurações", "Sair"};

    for (int i = 0; i < QUANT_BOTOES; i++) {
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

    // Inicializa tela de lore (mas não carrega textura ainda)
    telaLore = (TelaLore){
        .background = LoadTexture("assets/bg-historia.png"), // Usando um bg existente
        .textoHistoria = historiaSINT7,
        .scrollOffset = 0,
        .scrollSpeed = 20.0f,
        .podePular = false,
        .tempoExibicao = 0,
        .fonte = fonte
    };
}

void update_menu() {
    Vector2 mousePos = GetMousePosition();

    if (estadoAtual == MENU_PRINCIPAL) {
        for (int i = 0; i < QUANT_BOTOES; i++) {
            botoes[i].hover = CheckCollisionPointRec(mousePos, botoes[i].rect);

            if (botoes[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                switch (i) {
                    case 0: // Jogar
                        estadoAtual = MENU_LORE; // Vai para tela de lore primeiro
                        telaLore.scrollOffset = 0;
                        telaLore.podePular = false;
                        telaLore.tempoExibicao = 0;
                        break;
                    case 1: // Configurações
                        // Implemente conforme necessário
                        break;
                    case 2: // Sair
                        CloseWindow();
                        break;
                }
            }
        }
    }
    else if (estadoAtual == MENU_LORE) {
        // Atualiza rolagem do texto
        telaLore.scrollOffset += GetFrameTime() * telaLore.scrollSpeed;
        telaLore.tempoExibicao += GetFrameTime();
        
        // Permite pular após 2 segundos
        if (telaLore.tempoExibicao > 2.0f) {
            telaLore.podePular = true;
        }

        // Verifica se terminou ou se quer pular
        float textHeight = MeasureTextEx(telaLore.fonte, telaLore.textoHistoria, 24, 0).y;
        bool textoCompleto = telaLore.scrollOffset > (textHeight + GetScreenHeight());
        
        if ((telaLore.podePular && (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) || textoCompleto) {
            estadoAtual = MENU_JOGANDO;
            init_player();
        }
    }
    else if (estadoAtual == MENU_JOGANDO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_PAUSADO;
    }
    else if (estadoAtual == MENU_PAUSADO && IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = MENU_JOGANDO;
    }
}

void draw_menu() {
    switch (estadoAtual) {
        case MENU_PRINCIPAL:
            // Desenha background
            DrawTexturePro(
                backgroundTexture,
                (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0},
                0,
                WHITE
            );

            // Desenha botões
            for (int i = 0; i < QUANT_BOTOES; i++) {
                Color btnColor = botoes[i].hover ? SKYBLUE : (Color){200, 200, 200, 200};
                DrawRectangleRounded(botoes[i].rect, 0.3f, 10, btnColor);
                
                int textWidth = MeasureText(botoes[i].texto, 20);
                DrawText(
                    botoes[i].texto,
                    botoes[i].rect.x + (botoes[i].rect.width - textWidth) / 2,
                    botoes[i].rect.y + 15,
                    20,
                    DARKBLUE
                );
            }

            DrawText("© 2025 SINT-7 Game", 20, GetScreenHeight() - 30, 15, WHITE);
            break;

        case MENU_LORE:
            // Desenha background da lore
            DrawTexturePro(
                telaLore.background,
                (Rectangle){0, 0, telaLore.background.width, telaLore.background.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0},
                0,
                (Color){255, 255, 255, 200} // Com transparência
            );

            // Desenha texto da história com rolagem
            Vector2 textPos = {50, GetScreenHeight() - telaLore.scrollOffset};
            DrawTextEx(
                telaLore.fonte,
                telaLore.textoHistoria,
                textPos,
                24,
                0,
                WHITE
            );

            // Instrução para pular
            if (telaLore.podePular) {
                const char* skipText = "Pressione ENTER ou clique para continuar";
                int textWidth = MeasureText(skipText, 20);
                DrawText(
                    skipText,
                    (GetScreenWidth() - textWidth) / 2,
                    GetScreenHeight() - 50,
                    20,
                    WHITE
                );
            }
            break;

        case MENU_PAUSADO:
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
            const char* texto = "PAUSADO";
            int textWidth = MeasureText(texto, 40);
            DrawText(
                texto,
                (GetScreenWidth() - textWidth) / 2,
                GetScreenHeight() / 2 - 50,
                40,
                WHITE
            );
            
            const char* instrucao = "Pressione ESC para continuar";
            textWidth = MeasureText(instrucao, 20);
            DrawText(
                instrucao,
                (GetScreenWidth() - textWidth) / 2,
                GetScreenHeight() / 2 + 20,
                20,
                WHITE
            );
            break;

        default:
            break;
    }
}

void unload_menu_textures() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(telaLore.background);
    UnloadFont(fonte);
}

EstadoMenu get_estado_menu() {
    return estadoAtual;
}

void set_estado_menu(EstadoMenu estado) {
    estadoAtual = estado;
}