#include "utils.h"
#include "player.h"
#include "raylib.h"
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 512

Camera2D camera; 

void InitCamera(){
    camera = (Camera2D){ 0 };
    camera.target = (Vector2){ player.position.x + player.width/2, player.position.y + player.height/2 };
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}
