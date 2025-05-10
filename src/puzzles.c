#include "puzzles.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 900

Texture2D fragmentoTexture, bgfragmentoTexture;

FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS] = {
    { true, false, "\"O padrão era sempre primo.\nEla dizia: 2, 3, 5... o último era 7.\"", 1, ENIGMA },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA },
    { false, false, "\"Eu nasci do silência. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

//TO-DO: pegar onde a camera tá e colocar o conteudo do fragmento no top centro

void init_fragmento(){
    fragmentoTexture = LoadTexture("assets/fragmentos/Random Device 2.png");
    bgfragmentoTexture = LoadTexture("assets/fragmentos/background-frag.png");

    for (int i = 0; i<QUANT_FASES; i++){
        if(player.faseAtual == i+1){
            fragmentoObrigatorioAtual = fragmentosObrigatorios[i];
            fragmentoObrigatorioAtual.x = 550;
            fragmentoObrigatorioAtual.y = 350;
            char path[64];
            sprintf(path, "assets/fragmentos/background-frag/00%d.png", i+1);
            fragmentoObrigatorioAtual.texture = LoadTexture(path);
            //TO-DO: trocar imagens (texturas) para diferentes imagens (pode fazer uma lista com as texturas)
            break;
        }
        
    }
    
}

void draw_fragmento(){
    if (fragmentoObrigatorioAtual.fase == 1){
        Vector2 position = {550, 350};
        float scale = 3.0f;
        DrawTextureEx(fragmentoTexture, position, 0.0f, scale, WHITE);
    }
}

void check_colisao(){
    // HITBOX DO PLAYER
    Rectangle playerHitbox = {
        player.position.x,
        player.position.y,
        16 * 5,  // mesmo scale do draw_player
        16 * 5
    };

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

    if (CheckCollisionRecs(playerHitbox, fragmentoHitbox)) {
        DrawText("(F) para interagir", fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30, 20, GREEN);


        int fonteTamanho = 20;
        int textoLargura = MeasureText(fragmentoObrigatorioAtual.conteudo, fonteTamanho);
        int tituloLargura = MeasureText("Fragmento de Memória Encontrado", fonteTamanho);


        if (IsKeyDown(KEY_F)){ // ERRO: texto não mantem
            Vector2 position = {SCREEN_WIDTH / 2 - bgfragmentoTexture.width / 2, 20};
            float scale = 1.0f;
            DrawTextureEx(fragmentoObrigatorioAtual.texture, position, 0.0f, scale, WHITE);

            //DrawText("Fragmento de Memória Encontrado", (SCREEN_WIDTH - textoLargura) / 2, 40, 20, SKYBLUE);

            //DrawText(fragmentoObrigatorioAtual.conteudo, (SCREEN_WIDTH - textoLargura) / 2, 70, 18, WHITE);
        }
        
    }
}

void unload_fragmento() {
    UnloadTexture(fragmentoTexture);
}
