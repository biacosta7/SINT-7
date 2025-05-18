#include "raylib.h"
#include "player.h"
#include "utils.h"
#include "setup_puzzle.h"
#include "puzzles.h"
#include "decode_puzzle.h"
#include "game.h"
#include "graphics.h"
#include "fase.h"
#include "fragmentos.h"
#include "ia.h"
#include "../include/menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern Camera2D camera; 
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Color cianoNeon = (Color){0, 217, 224, 255};
int scrollOffset = 0;

int main() {
    InitAudioDevice(); 
    Music trilhaSonora = LoadMusicStream("assets/sound/trilha_sonora.wav");
    PlayMusicStream(trilhaSonora); //adicionar a trilha sonora e colocar ela no parenteses a cima
    
    vcr = LoadFont("assets/VCR.ttf");

    bool inventarioAberto = false;
    bool aba_comandos_aberto = false;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SINT-7");
    SetTargetFPS(60);

    init_player();
    InitGraphics();
    InitCamera();
    init_fase();
    init_menu();

    Texture2D botaoTexture = LoadTexture("assets/fragmentos/background-frag/botao.png");
    Texture2D inventarioTexture = LoadTexture("assets/fragmentos/background-frag/inventario.png");

    for (int i = 0; i < 4; i++) {
        printf("$OI AQUI: Conteúdo: %s | Sentimento: %d\n", fragmentos[i].conteudo, fragmentos[i].sentimento);
    }

    while (!WindowShouldClose()) {
        UpdateMusicStream(trilhaSonora);
        update_menu();

        if (get_estado_menu() == MENU_JOGANDO) {
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 2;
            if (IsKeyDown(KEY_LEFT))  player.position.x -= 2;

            update_player();
            UpdateCameraPosition();
            UpdateCameraMove();
            update_fragmento();
            update_puzzle();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (get_estado_menu() == MENU_JOGANDO || get_estado_menu() == MENU_PAUSADO) {
            BeginMode2D(camera);
                DrawAllBackgrounds();
                draw_fragmentos();
                char interacao = check_colisoes();
                atualizar_e_desenhar_fundo_escuro();
            EndMode2D();

            int btnX = 50;
            int btnY = 50;
            float scale = 0.1f;
            float scaledWidth = botaoTexture.width * scale;
            float scaledHeight = botaoTexture.height * scale;

            Vector2 btnInventarioPos = {btnX, btnY};
            Vector2 btnComandosPos = {btnX + scaledWidth + 10, btnY};

            DrawTextureEx(botaoTexture, btnInventarioPos, 0.0f, scale, WHITE);
            DrawTextureEx(botaoTexture, btnComandosPos, 0.0f, scale, WHITE);

            if(deveDesenharEscuro) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                              (Color){0, 0, 0, (unsigned char)escurecimentoAlpha});
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();

                if (mouse.x >= btnInventarioPos.x && mouse.x <= btnInventarioPos.x + scaledWidth &&
                    mouse.y >= btnInventarioPos.y && mouse.y <= btnInventarioPos.y + scaledHeight) {
                    inventarioAberto = !inventarioAberto;
                    alternar_estado_fundo_escuro(inventarioAberto);
                    printf("Botão Inventário clicado!\n");
                } else if (mouse.x >= btnComandosPos.x && mouse.x <= btnComandosPos.x + scaledWidth &&
                           mouse.y >= btnComandosPos.y && mouse.y <= btnComandosPos.y + scaledHeight) {
                    aba_comandos_aberto = !aba_comandos_aberto;
                    printf("Botão Comandos clicado!\n");
                }
            }

            if (inventarioAberto) {
                int scrollSpeed = 25;

                // Handle scroll input
                if (IsKeyPressed(KEY_DOWN)) {
                    scrollOffset -= scrollSpeed;
                    printf("Seta para baixo pressionada! scrollOffset: %d\n", scrollOffset);
                }
                if (IsKeyPressed(KEY_UP)) {
                    scrollOffset += scrollSpeed;
                    printf("Seta para cima pressionada! scrollOffset: %d\n", scrollOffset);
                }

                int scaleI = 5.0f;
                int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
                int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;

                // Desenha a textura do inventário
                DrawTextureEx(inventarioTexture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);

                // Calcula dimensões da área de texto
                int margem = 30;
                int areaTextoAltura = inventarioTexture.height * scaleI - 60; // altura da scissor area
                
                // PRIMEIRO: Calcular altura total do conteúdo sem desenhar
                int textoAlturaTotal = 0;
                int linhaAltura = 25;
                NodeFragmento *temp = fragmentosColetados;
                int contador = 1;
                
                while (temp != NULL) {
                    // Header do fragmento
                    textoAlturaTotal += linhaAltura;
                    
                    // Calcula quantas linhas o conteúdo vai ocupar
                    char textoFormatado[1024];
                    int larguraDisponivel = inventarioTexture.width * scaleI - 2 * margem;
                    QuebrarTextoPorLargura(temp->fragmento.conteudo, textoFormatado, larguraDisponivel, 20);
                    
                    // Conta as quebras de linha
                    char *copia = strdup(textoFormatado); // Cria uma cópia para não afetar o strtok original
                    char *linha = strtok(copia, "\n");
                    while (linha != NULL) {
                        textoAlturaTotal += linhaAltura;
                        linha = strtok(NULL, "\n");
                    }
                    free(copia);
                    
                    textoAlturaTotal += 10; // Espaço entre fragmentos
                    temp = temp->next;
                    contador++;
                }
                
                // Calcula os limites do scroll
                int scrollLimiteInferior = areaTextoAltura - textoAlturaTotal;
                if (scrollLimiteInferior > 0) scrollLimiteInferior = 0; // Se o conteúdo cabe na tela
                
                // Aplica os limites ao scrollOffset
                if (scrollOffset > 0) scrollOffset = 0; // Não pode passar do topo
                if (scrollOffset < scrollLimiteInferior) scrollOffset = scrollLimiteInferior; // Não pode passar do fundo
                
                // AGORA: Desenhar o conteúdo com scroll aplicado
                BeginScissorMode(
                    inventX + margem,
                    inventY + 30,
                    (int)(inventarioTexture.width * scaleI - 2 * margem),
                    areaTextoAltura
                );
                
                int i = 1;
                int textoX = inventX + 40;
                int textoY = inventY + 50 + scrollOffset; // Aplica o offset
                NodeFragmento *atual = fragmentosColetados;
                
                while (atual != NULL) {
                    // Desenha o header do fragmento
                    DrawText(TextFormat("FM-00%d:", i), textoX, textoY, 20, cianoNeon);
                    textoY += linhaAltura;

                    // Quebra e desenha o conteúdo
                    char textoFormatado[1024];
                    int larguraDisponivel = inventarioTexture.width * scaleI - 2 * margem;
                    QuebrarTextoPorLargura(atual->fragmento.conteudo, textoFormatado, larguraDisponivel, 20);

                    char *linha = strtok(textoFormatado, "\n");
                    while (linha != NULL) {
                        DrawText(linha, textoX + 10, textoY, 20, WHITE);
                        textoY += linhaAltura;
                        linha = strtok(NULL, "\n");
                    }

                    textoY += 10; // Espaço entre fragmentos
                    atual = atual->next;
                    i++;
                }

                EndScissorMode();
                
                // Debug info (opcional)
                printf("Altura total do conteúdo: %d, Área disponível: %d, Scroll: %d/%d\n", 
                    textoAlturaTotal, areaTextoAltura, scrollOffset, scrollLimiteInferior);
            }

            if (aba_comandos_aberto) {
                int imgX = (SCREEN_WIDTH - inventarioTexture.width) / 2;
                int imgY = (SCREEN_HEIGHT - inventarioTexture.height) / 2;

                int scaleI = 5.0f;
                int inventX = (SCREEN_WIDTH - inventarioTexture.width * scaleI) / 2;
                int inventY = (SCREEN_HEIGHT - inventarioTexture.height * scaleI) / 2;
                DrawTextureEx(inventarioTexture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);
            }

            if (puzzleFoiAtivado) {
                draw_puzzle(player.faseAtual);
                if (IsKeyDown(KEY_X)) {
                    puzzleFoiAtivado = false;
                    init_puzzle(player.faseAtual);
                    alternar_estado_fundo_escuro(false);
                }
            }

            if (fragmentoFoiAtivado) {
                draw_fragmento(fragmentoObrigatorioAtual.fase);
            }
            
            if (fragmentoOpcionalFoiAtivado) {
                draw_fragmento_opcional(fragmentoObrigatorioAtual.fase);
            }

            DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 30, 20, WHITE);
            //puzzle_decode();

            BeginMode2D(camera);
                draw_player();
            EndMode2D();
            
        }

        if (get_estado_menu() != MENU_JOGANDO) {
            draw_menu();
        }
        
        EndDrawing();
    }
    UnloadMusicStream(trilhaSonora);
    CloseAudioDevice(); 
    unload_menu_textures();
    UnloadGraphics();
    UnloadTexture(botaoTexture);
    UnloadTexture(inventarioTexture);
    free_player_resources();
    UnloadFont(vcr);
    CloseWindow();
    return 0;
}
