#include "utils.h"
#include "player.h"
#include "raylib.h"
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Camera2D camera; 

void InitCamera(){
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ player.position.x + player.width/2, player.position.y + player.height/2 };
    camera.offset = (Vector2){ 10, SCREEN_HEIGHT / 2.0f };  // Jogador 100px da esquerda
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
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
