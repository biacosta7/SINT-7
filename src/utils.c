#include "utils.h"
#include "player.h"
#include "raylib.h"
#include "config.h"

float cameraX = 0.0f;
 
void InitCamera(){
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ player.position.x + player.width/2, player.position.y + player.height/2 };
    camera.offset = (Vector2){ 10, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void UpdateCameraMove() {
    cameraX = camera.target.x - SCREEN_WIDTH / 2;

    if (cameraX < 0) cameraX = 0;

    float maxCamera = (SECTOR_WIDTH * SECTOR_COUNT) - SCREEN_WIDTH;
    if (cameraX > maxCamera) cameraX = maxCamera;

    if (!player.fasesDesbloqueadas[1] && camera.target.x > 630) {
        cameraX = 630 - SCREEN_WIDTH / 2;
    } 

    if (!player.fasesDesbloqueadas[2] && camera.target.x > 2690) {
        cameraX = 2690 - SCREEN_WIDTH / 2;
    }    
}

void UpdateCameraPosition() {
    float playerScreenX = player.position.x - camera.target.x + camera.offset.x;

    const float leftMargin = 200;
    const float rightMargin = SCREEN_WIDTH - 200;

    if (playerScreenX < leftMargin) {
        camera.target.x = player.position.x - leftMargin + camera.offset.x;
    }
    else if (playerScreenX > rightMargin) {
        camera.target.x = player.position.x - rightMargin + camera.offset.x;
    }

    camera.target.y = SCREEN_HEIGHT / 2.0f;
}