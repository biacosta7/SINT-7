#include "player.h"
#include <stdbool.h>

Player player; 
extern float cameraX;  // Adicione no topo de player.c


static int frame = 0;
static float frameTime = 0.1f;
static float frameTimer = 0.0f;
float scale = 5.0f;  // Aumenta 5x (de 16x16 para 64x64 pixels)


void init_player() {
    player.position = (Vector2){ 200, 398 };
    player.idleTexture = LoadTexture("assets/edu/idle.png");
    player.walkTexture = LoadTexture("assets/edu/walk.png");
    player.vida = 3;

    player.maxFrames = 3; 
}

void update_player() {

    frameTimer += GetFrameTime();
    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        player.frame = (player.frame + 1) % player.maxFrames;  // Loop de 0 a 2
    }

    if(IsKeyDown(KEY_RIGHT)){
        player.state = WALK;
        player.position.x += 2;
        player.direction = 1;

        if (player.position.x > 1430){ // && não desbloqueou a fase 2 (fazer a mesma condicional p/outras fases, pode ser um loop de verificacao)
            player.position.x = 1430;
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
        source.x = (player.frame + 1) * 16; // ajuste para espelhamento
        source.width = -16; // espelha horizontalmente
    }

    Rectangle dest = { player.position.x, player.position.y, 16 * scale, 16 * scale };
    Vector2 origin = { 0, 0 };
    float rotation = 0.0f;

    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}



void free_player_resources() {
    UnloadTexture(player.idleTexture);
}
