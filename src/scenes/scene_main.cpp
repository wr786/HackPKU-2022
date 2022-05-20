#include "raylib.h"
#include "scene.h"

class SceneMain: public SceneBase {
private:
    bool isEnd = false;
public:
    void init() {
        printf("[debug] calling SceneMain");
        //const int screenWidth = 1600;
        //const int screenHeight = 900;

        SetTargetFPS(60);
    }
    void draw() {
        BeginDrawing();
            ClearBackground(GRAY);
            DrawTriangle({1600, 400}, {1440, 900}, {1600, 900}, Fade(BLACK, 0.2f));
        EndDrawing();
    }
    void update() {
        //====================键盘操控=================
        if(IsKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }
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