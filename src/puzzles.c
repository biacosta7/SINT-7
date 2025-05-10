#include "puzzles.h"
#include "player.h"

Texture2D fragmentoTexture;

FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS] = {
    { true, false, "O padrão era sempre primo. Ela dizia: 2, 3, 5… o último era 7.", 1, ENIGMA },
    { true, false, "A senha era simples: 0101, como sempre.", 2, ENIGMA },
    { false, false, "O módulo de cálculo priorizava a eficiência. O módulo de empatia... falhava com frequência, mas nos fazia sorrir.", 3, ENIGMA },
    { false, false, "Eu nasci do silência. Depois me conectaram. O mundo doeu. Então me calaram.", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

//TO-DO: pegar onde a camera tá e colocar o conteudo do fragmento no top centro

void init_fragmento(){
    fragmentoTexture = LoadTexture("assets/fragmentos/Random Device 2.png");

    for (int i = 0; i<QUANT_FASES; i++){
        if(player.faseAtual == i+1){
            fragmentoObrigatorioAtual = fragmentosObrigatorios[i];
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

void unload_fragmento() {
    UnloadTexture(fragmentoTexture);
}
