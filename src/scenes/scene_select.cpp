#include "raylib.h"
#include "scene.h"
#include "animation.h"
#include "../config.cpp"
#include "../music.cpp"
#include <cstring>
#include <iostream>
using namespace std;

class SceneSelect: public SceneBase {
private:
    bool isEnd = false;    
    vector<Music> BGMlst;
    vector<Texture2D> illustLst;
    bool curMusicIndex;
    // 背景
    Animation bg;
    Rectangle windowRec;
    float rotation;
public:
    void init() {
        printf("[debug] calling SceneSelect");

        InitAudioDevice();
        init_BGM_play();

        // 加载音乐列表
        init_music_status_list();
        for(auto& ms: musicStatus_list) {
            cout << ms.fullName() << endl;
            string path = MUSIC_FOLDER + ms.fullName() + ".wav";
            BGMlst.push_back(LoadMusicStream(path.c_str()));
            path = SONG_ILLUST_FOLDER + ms.fullName() + ".png";
            illustLst.push_back(LoadTexture(path.c_str()));
        }

        curMusicIndex = 0;
        rotation = 0;

        // 初始化背景
        bg = Animation(IMAGE_FOLDER + "bg_select.png", 5, 5, 12);
        windowRec = {0, 0, SceneSelect::screenWidth, SceneSelect::screenHeight};
        
        SetTargetFPS(60);
    }
    void draw() {
        BeginDrawing();
            ClearBackground(GRAY);
            // 处理背景
            DrawTexturePro(bg.getTexture(), bg.getFrame(), windowRec, {0.f, 0.f}, 0, WHITE);
            // 中间CD
            DrawTexturePro(
                illustLst[curMusicIndex], 
                {0, 0, (float)illustLst[curMusicIndex].width, (float)illustLst[curMusicIndex].height}, 
                {SceneSelect::screenWidth/2, SceneSelect::screenHeight/2, (float)illustLst[curMusicIndex].width, (float)illustLst[curMusicIndex].height},
                {(float)illustLst[curMusicIndex].width / 2, (float)illustLst[curMusicIndex].height / 2}, 
                (float)rotation, 
                WHITE
            );
            
            DrawTriangle({1600, 400}, {1440, 900}, {1600, 900}, Fade(BLACK, 0.2f));
        EndDrawing();
    }
    void update() {
        // 播放音乐
        play_repeat(BGMlst[curMusicIndex]);

        bg.nextFrame();
        rotation+=0.5;
        //====================键盘操控=================
        if(IsKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }
    }
    SceneType end() {
        for(auto& music: BGMlst) {
            UnloadMusicStream(music);
        }
        for(auto& illust: illustLst) {
            UnloadTexture(illust);
        }
        bg.unload();

        return SCENE_MAIN;
    }
    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
