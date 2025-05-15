#include "fase.h"
#include "puzzles.h"
#include "player.h"

void init_fase(){
    for (int i = 0; i<QUANT_FASES; i++){
        
        if(player.faseAtual == i+1){
            faseAtual.numero = player.faseAtual;
            faseAtual.foiConcluida = false;
            faseAtual.fragmentoObrigatorio = fragmentosObrigatorios[i];
            faseAtual.puzzle = puzzles[i];

            init_fragmento(faseAtual.numero);
            init_puzzle(faseAtual.numero);

            break;
        }
        
    }
}