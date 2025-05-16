#include "game.h"
#include "player.h"
#include "fragmentos.h"
#include "setup_puzzle.h"

char check_colisoes(){
    // HITBOX DO PLAYER
    Rectangle playerHitbox = {
        player.position.x,
        player.position.y,
        16 * 5,  // mesmo scale do draw_player
        16 * 5
    };
    bool fragmento = check_colisao_fragmento(playerHitbox);
    bool puzzle = check_colisao_puzzle(playerHitbox);
    //bool fragmento_opcional = check_colisao_fragmento_opcional(playerHitbox);

    if (fragmento) return 'f';
    else if (puzzle) return 'p';
    //else if(fragmento_opcional) return 'o';
    return 'z';
}