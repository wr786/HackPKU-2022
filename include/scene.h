#ifndef __SCENE_H__
#define __SCENE_H__
#include "raylib.h"
#include <string>
using namespace std;

typedef enum {
    SCENE_NULL = -1, 
    SCENE_MAIN, 
    SCENE_SELECT,
    SCENE_PLAY,
    SCENE_TOTAL
} SceneType;


class SceneBase {
    public:
        static const int screenWidth = 1600;
        static const int screenHeight = 900;

        virtual void init();
        virtual void update();
        virtual void draw();
        virtual SceneType end();
        virtual bool is_end();
};

#endif