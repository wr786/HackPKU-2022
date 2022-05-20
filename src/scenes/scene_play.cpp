#include "raylib.h"
#include "scene.h"
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define NOTE_SPEED 5

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
};

class Notes{
public:
    float time;
    int rail;
    Color color;
    Rectangle bounds;
};


class ScenePlay: public SceneBase {

private:
    bool isEnd = false;
    Player *player;
    
public:
    void init() {
        printf("[debug] calling ScenePlay");

        SetTargetFPS(60);

        player = new Player();
        player->rail = 0;
        player->color = RED;
        player->bounds = (Rectangle){ 30 + 14, player->rail * 120 + 200, PLAYER_WIDTH, PLAYER_HEIGHT };
    }

    void draw() {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(player->bounds.x, player->bounds.y, PLAYER_WIDTH, PLAYER_HEIGHT, player->color);
        EndDrawing();
    }

    void update() {
        //====================键盘操控=================
        if(IsKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_F)) 
            player->rail -= 1;
        if (IsKeyDown(KEY_J) || IsKeyDown(KEY_K)) 
            player->rail += 1;

        // Check player not out of rails
        if (player->rail > 1) 
            player->rail = 1;
        else if (player->rail < 0) 
            player->rail = 0;

        printf("[debug]Player rail: %d\n", player->rail);
        player->bounds = (Rectangle){ 30 + 14, player->rail * 120 + 200, PLAYER_WIDTH, PLAYER_HEIGHT };
    }

    SceneType end() {
        return SCENE_NULL;
    }

    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};