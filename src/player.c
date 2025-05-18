#include "player.h"
#include <stdbool.h>

Player player; 
extern float cameraX; 

static int frame = 0;
static float frameTime = 0.1f;
static float frameTimer = 0.0f;
float scale = 5.0f;

void init_player() {
    player.position = (Vector2){ 200, 398 };
    player.idleTexture = LoadTexture("assets/edu/idle.png");
    player.walkTexture = LoadTexture("assets/edu/walk.png");
    player.faseAtual = 1;
    player.maxFrames = 3;
    
    player.fasesDesbloqueadas[0] = true;
    for (int i = 1; i < QUANT_FASES; i++) {
        player.fasesDesbloqueadas[i] = false;
    }
}

void desbloquear_fase(int fase) {
    if (fase > 0 && fase < QUANT_FASES) {
        player.fasesDesbloqueadas[fase] = true;
        player.faseAtual = fase + 1;
        printf("Fase %d desbloqueada!\n", fase + 1);
    }
}

void update_player() {
    frameTimer += GetFrameTime();
    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        player.frame = (player.frame + 1) % player.maxFrames;
    }

    if(IsKeyDown(KEY_RIGHT)){
        player.state = WALK;
        player.position.x += 2;
        player.direction = 1;

        if (!player.fasesDesbloqueadas[1] && player.position.x > 1430) {
            player.position.x = 1430;
        }
        if (!player.fasesDesbloqueadas[2] && player.position.x > 3410) {
            cameraX = 3410 - SCREEN_WIDTH / 2;
        }  
    }
    else if (IsKeyDown(KEY_LEFT)){
        player.state = WALK;
        player.position.x -= 2;
        player.direction = -1;

        if (player.position.x < 200){
            player.position.x = 200;
        }
    }
    else{
        player.state = IDLE;    
    }
}

void draw_player() {
    Texture2D texture;
    switch(player.state){
        case WALK:
            texture = player.walkTexture;
            break;
        case IDLE:
        default:
            texture = player.idleTexture;
            break;
    }
    int direction = player.direction;

    Rectangle source;
    source.y = 0;
    source.width = 16;
    source.height = 16;

    if (direction == 1) {
        source.x = player.frame * 16;
    } else {
        source.x = (player.frame + 1) * 16;
        source.width = -16;
    }

    Rectangle dest = { player.position.x, player.position.y, 16 * scale, 16 * scale };
    Vector2 origin = { 0, 0 };
    float rotation = 0.0f;

    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void free_player_resources() {
    UnloadTexture(player.idleTexture);
    UnloadTexture(player.walkTexture);
}