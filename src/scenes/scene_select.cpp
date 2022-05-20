#include "raylib.h"
#include "scene.h"
#include "config.h"
#include "../music.cpp"

class SceneSelect: public SceneBase {
private:
    bool isEnd = false;    
    vector<Music> BGMlst;
    bool curMusicIndex;
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

        SetTargetFPS(60);
    }
    void draw() {
        // 播放音乐
        play_repeat(BGMlst[curMusicIndex]);

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
        return SCENE_MAIN;
    }
    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
