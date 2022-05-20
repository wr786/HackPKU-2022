#include "scene.h"
#include "raylib.h"
#include "scenes/scene_main.cpp"
#include <cstdio>
#include <string>
using std::string;

SceneType sceneType = SCENE_MAIN;
SceneBase* scenes[SCENE_TOTAL];

int main()
{
    /* Register scenes */
    scenes[SCENE_MAIN] = new SceneMain();
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