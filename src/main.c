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
                DrawBackground();
                draw_fragmento_trigger();
                draw_fragmento_opcional_trigger();
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
                    printf("Botão Inventário clicado!\n");
                } else if (mouse.x >= btnComandosPos.x && mouse.x <= btnComandosPos.x + scaledWidth &&
                           mouse.y >= btnComandosPos.y && mouse.y <= btnComandosPos.y + scaledHeight) {
                    aba_comandos_aberto = !aba_comandos_aberto;
                    printf("Botão Comandos clicado!\n");
                }
            }

            if (inventarioAberto) {
                int scrollSpeed = 25;

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
                    int larguraDisponivel = inventarioTexture.width * scaleI - 2 * margem - 20; // -20 para a scrollbar
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
                
                // Adiciona margem extra no final para garantir que a última linha seja totalmente visível
                textoAlturaTotal += 30;
                
                // Calcula os limites do scroll
                int scrollLimiteInferior = areaTextoAltura - textoAlturaTotal;
                if (scrollLimiteInferior > 0) scrollLimiteInferior = 0; // Se o conteúdo cabe na tela
                
                // Calcula se precisa mostrar a scrollbar
                bool precisaScroll = textoAlturaTotal > areaTextoAltura;
                
                // HANDLE SCROLL WITH MOUSE
                if (precisaScroll) {
                    // Posições da área da scrollbar
                    int scrollbarX = inventX + inventarioTexture.width * scaleI - margem - 15;
                    int scrollbarY = inventY + 35;
                    int scrollbarLargura = 15;
                    int scrollbarAltura = areaTextoAltura - 10;
                    
                    // Mouse wheel scroll
                    float mouseWheel = GetMouseWheelMove();
                    if (mouseWheel != 0) {
                        scrollOffset += (int)(mouseWheel * scrollSpeed);
                        printf("Mouse wheel! Novo scrollOffset: %d\n", scrollOffset);
                    }
                    
                    // Click and drag scrollbar
                    Vector2 mousePos = GetMousePosition();
                    static bool isDragging = false;
                    static int dragStartY = 0;
                    static int dragStartOffset = 0;
                    
                    // Verifica se o mouse está sobre a área da scrollbar
                    Rectangle scrollbarRect = {scrollbarX, scrollbarY, scrollbarLargura, scrollbarAltura};
                    bool mouseOverScrollbar = CheckCollisionPointRec(mousePos, scrollbarRect);
                    
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverScrollbar) {
                        isDragging = true;
                        dragStartY = (int)mousePos.y;
                        dragStartOffset = scrollOffset;
                        printf("Iniciou drag na scrollbar!\n");
                    }
                    
                    if (isDragging) {
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                            // Calcula o movimento do mouse
                            int mouseDelta = (int)mousePos.y - dragStartY;
                            
                            // Converte o movimento do mouse para movimento de scroll
                            float scrollRange = (float)(-scrollLimiteInferior);
                            float scrollbarRange = (float)(scrollbarAltura);
                            float scrollRatio = scrollRange / scrollbarRange;
                            
                            int newScrollOffset = dragStartOffset - (int)(mouseDelta * scrollRatio);
                            scrollOffset = newScrollOffset;
                            
                            printf("Dragging... offset: %d\n", scrollOffset);
                        } else {
                            isDragging = false;
                            printf("Terminou drag\n");
                        }
                    }
                    
                    // Click direto na scrollbar (não drag)
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverScrollbar && !isDragging) {
                        // Calcula onde foi clicado na scrollbar
                        float clickPercent = ((float)mousePos.y - (float)scrollbarY) / (float)scrollbarAltura;
                        
                        // Move o scroll para essa posição
                        scrollOffset = -(int)(clickPercent * (-scrollLimiteInferior));
                        printf("Click direto na scrollbar! offset: %d\n", scrollOffset);
                    }
                }
                
                // Aplica os limites ao scrollOffset
                if (scrollOffset > 0) scrollOffset = 0; // Não pode passar do topo
                if (scrollOffset < scrollLimiteInferior) scrollOffset = scrollLimiteInferior; // Não pode passar do fundo
                
                // AGORA: Desenhar o conteúdo com scroll aplicado
                int areaTextoLargura = inventarioTexture.width * scaleI - 2 * margem - (precisaScroll ? 20 : 0);
                
                BeginScissorMode(
                    inventX + margem,
                    inventY + 30,
                    areaTextoLargura,
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
                    QuebrarTextoPorLargura(atual->fragmento.conteudo, textoFormatado, areaTextoLargura - 80, 20);

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
                
                // DESENHAR A BARRA DE SCROLL
                if (precisaScroll) {
                    // Posições da área da scrollbar
                    int scrollbarX = inventX + inventarioTexture.width * scaleI - margem - 15;
                    int scrollbarY = inventY + 35;
                    int scrollbarLargura = 15;
                    int scrollbarAltura = areaTextoAltura - 10;
                    
                    // Desenha o fundo da scrollbar (trilho)
                    DrawRectangle(scrollbarX, scrollbarY, scrollbarLargura, scrollbarAltura, 
                                (Color){40, 40, 40, 180}); // Cinza escuro semi-transparente
                    
                    // Calcula o tamanho e posição do "thumb" (indicador)
                    float scrollRatio = (float)areaTextoAltura / (float)textoAlturaTotal;
                    int thumbAltura = (int)(scrollbarAltura * scrollRatio);
                    
                    // Garante um tamanho mínimo para o thumb
                    if (thumbAltura < 20) thumbAltura = 20;
                    
                    // Calcula a posição do thumb baseada no scroll atual
                    float scrollProgress = 0.0f;
                    if (scrollLimiteInferior != 0) {
                        scrollProgress = (float)(-scrollOffset) / (float)(-scrollLimiteInferior);
                    }
                    
                    int thumbPosY = scrollbarY + (int)((scrollbarAltura - thumbAltura) * scrollProgress);
                    
                    // Verifica se o mouse está sobre a scrollbar para feedback visual
                    Vector2 mousePos = GetMousePosition();
                    Rectangle scrollbarRect = {scrollbarX, scrollbarY, scrollbarLargura, scrollbarAltura};
                    bool mouseOverScrollbar = CheckCollisionPointRec(mousePos, scrollbarRect);
                    
                    // Desenha o thumb com feedback visual
                    Color thumbColor = mouseOverScrollbar ? 
                        (Color){cianoNeon.r + 30, cianoNeon.g + 30, cianoNeon.b + 30, 255} : 
                        cianoNeon;
                        
                    DrawRectangle(scrollbarX + 2, thumbPosY, scrollbarLargura - 4, thumbAltura, thumbColor);
                    
                    // Adiciona um brilho sutil no thumb
                    DrawRectangleLines(scrollbarX + 1, thumbPosY - 1, scrollbarLargura - 2, thumbAltura + 2, 
                                    (Color){thumbColor.r, thumbColor.g, thumbColor.b, 100});
                    
                    // Desenha bordas da scrollbar
                    DrawRectangleLines(scrollbarX, scrollbarY, scrollbarLargura, scrollbarAltura, 
                                    (Color){100, 100, 100, 180});
                }
                
                // Debug info (opcional)
                // printf("Altura total: %d, Área: %d, Scroll: %d/%d\n", 
                //       textoAlturaTotal, areaTextoAltura, scrollOffset, scrollLimiteInferior);
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
    UnloadFont(cartograph);
    free_fragmento_resources();
    CloseWindow();
    return 0;
}
