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
    bool puzzle = check_colisao_puzzle(playerHitbox);
    checar_colisoes_fragmentos(playerHitbox);

    if (puzzle) return 'p';
    return 'z';
}