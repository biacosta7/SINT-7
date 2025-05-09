#include "utils.h"
#include "player.h"
#include "raylib.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512
#define SECTOR_COUNT 2
#define SECTOR_WIDTH 900
#define SECTOR_HEIGHT 512

float cameraX = 0.0f;
Camera2D camera;  

void InitCamera(){
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ player.position.x + player.width/2, player.position.y + player.height/2 };
    camera.offset = (Vector2){ 10, SCREEN_HEIGHT / 2.0f };  // Jogador 100px da esquerda
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void UpdateCameraMove() {
    cameraX = camera.target.x - SCREEN_WIDTH / 2;

    if (cameraX < 0) cameraX = 0;

    float maxCamera = (SECTOR_WIDTH * SECTOR_COUNT) - SCREEN_WIDTH;
    if (cameraX > maxCamera) cameraX = maxCamera;

    if (camera.target.x > 630){ // && não desbloqueou a fase 2 (fazer a mesma condicional p/outras fases, pode ser um loop de verificacao) - fase 1 limite : player.position.x = 1368 | camera.target.x = 678 | player pode andar até 1400 na primeira fase se a segunda nao tiver desbloqueada 
        cameraX = 630 - SCREEN_WIDTH / 2;
    }    
}


void UpdateCameraPosition() {
    float playerScreenX = player.position.x - camera.target.x + camera.offset.x;

    const float leftMargin = 200;
    const float rightMargin = SCREEN_WIDTH - 200;

    // Se o jogador passar da margem esquerda
    if (playerScreenX < leftMargin) {
        camera.target.x = player.position.x - leftMargin + camera.offset.x;
    }

    // Se o jogador passar da margem direita
    else if (playerScreenX > rightMargin) {
        camera.target.x = player.position.x - rightMargin + camera.offset.x;
    }

    camera.target.y = SCREEN_HEIGHT / 2.0f;

}
