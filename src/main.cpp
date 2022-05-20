#include "scene.h"
#include "raylib.h"
#include "scenes/scene_main.cpp"
#include "scenes/scene_select.cpp"
#include "scenes/scene_play.cpp"
#include <cstdio>
#include <string>
using std::string;

// 修改下面这行即可修改起始界面，方面测试
SceneType sceneType = SCENE_PLAY;
SceneBase* scenes[SCENE_TOTAL];

int main()
{
    /* Register scenes */
    scenes[SCENE_MAIN] = new SceneMain();
    scenes[SCENE_SELECT] = new SceneSelect();
    scenes[SCENE_PLAY] = new ScenePlay();
    SceneBase* g = scenes[sceneType];

    InitWindow(g->screenWidth, g->screenHeight, "Unnamed");
#ifdef DEBUG
    printf("[debug] init window successful!\n");
#endif
    g->init();

    SetTargetFPS(60);
    InitAudioDevice();
    //Main game loop
    //Detect window close button or ESC key
    while (!WindowShouldClose()) 
    {
        /* Update */
        g->update();
        if(g->is_end()){
            #ifdef DEBUG
            printf("[debug] switching to scene %d\n", sceneType);
            #endif
            sceneType=g->end();
            g = scenes[sceneType];
            g->init();
        }
        else {
            /* Draw */
            g->draw();
        }
    }
    //Close window and OpenGL context
    CloseWindow();
}