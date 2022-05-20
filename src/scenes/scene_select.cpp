#include "raylib.h"
#include "scene.h"
#include "animation.h"
#include "config.h"
#include "../music.cpp"
#include <cstring>
#include <iostream>
using namespace std;

class SceneSelect: public SceneBase {
private:
    bool isEnd = false;    
    vector<Music> BGMlst;
    bool curMusicIndex;
    // 背景
    Animation bg;
    Rectangle windowRec;
public:
    void init() {
        printf("[debug] calling SceneSelect");

        InitAudioDevice();
        init_BGM_play();

        // 加载音乐列表
        init_music_list();
        for(auto& mname: music_list) {
            string path = MUSIC_FOLDER + mname;
            BGMlst.push_back(LoadMusicStream(path.c_str()));
        }

        curMusicIndex = 0;

        // 初始化背景
        bg = Animation(string(IMAGE_FOLDER) + string("bg_select.png"), 5, 5, 12);
        windowRec = {0, 0, SceneSelect::screenWidth, SceneSelect::screenHeight};

        SetTargetFPS(60);
    }
    void draw() {
        // 播放音乐
        play_repeat(BGMlst[curMusicIndex]);

        BeginDrawing();
            ClearBackground(GRAY);
            // 处理背景
            bg.nextFrame();
            DrawTexturePro(bg.getTexture(), bg.getFrame(), windowRec, {0.f, 0.f}, 0, WHITE);
            
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
        for(auto& music: BGMlst) {
            UnloadMusicStream(music);
        }

        return SCENE_MAIN;
    }
    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
