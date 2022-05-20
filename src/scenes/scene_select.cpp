#include "raylib.h"
#include "scene.h"
#include "animation.h"
#include "../config.cpp"
#include "../music.cpp"
#include <cstring>
#include <iostream>
using namespace std;

extern MusicStatus selectedMusicStatus;

class SceneSelect: public SceneBase {
private:
    bool isEnd = false;    
    bool inited = false;
    int escCnt = 2;
    vector<Music> BGMlst;
    vector<Texture2D> illustLst;
    int curMusicIndex;
    Animation bg;   // 背景
    Rectangle windowRec;
    float rotation;
    Font fontCaption;
    int selectedMusicIndex;

    bool isKeyPressed(KeyboardKey key) {
        if(IsKeyPressed(key)) {
            play_once(taps[0]);
            return true;
        }
        return false;
    }
public:
    void init() {
        printf("[debug] calling SceneSelect");

        if(!inited) {
            InitAudioDevice();
            init_BGM_play();
            init_taps();

            // 加载音乐列表
            init_music_status_list();
            for(auto& ms: musicStatus_list) {
                string path = MUSIC_FOLDER + ms.fullName() + ".wav";
                BGMlst.push_back(LoadMusicStream(path.c_str()));
                path = SONG_ILLUST_FOLDER + ms.fullName() + ".png";
                illustLst.push_back(LoadTexture(path.c_str()));
            }

            // 初始化背景
            bg = Animation(IMAGE_FOLDER + "bg_select_2.png", 5, 5, 12);
            windowRec = {0, 0, screenWidth, screenHeight};
            
            fontCaption = LoadFontEx(string(FONT_FOLDER + "bb2180.ttf").c_str(), 96, 0, 0);

            inited = true;
        }

        if(selectedMusicIndex) curMusicIndex = selectedMusicIndex;
        else curMusicIndex = 0;
        selectedMusicIndex = -1;
        rotation = 0;

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
                {screenWidth/2, screenHeight/2, (float)illustLst[curMusicIndex].width, (float)illustLst[curMusicIndex].height},
                {(float)illustLst[curMusicIndex].width / 2, (float)illustLst[curMusicIndex].height / 2}, 
                (float)rotation, 
                WHITE
            );

            // topBar
            DrawRectangle(screenWidth/4, 0, screenWidth/2, screenHeight/8, Fade(WHITE, 0.8f));
            DrawTriangle({screenWidth/4-screenWidth/16, 0}, {screenWidth/4, screenHeight/8}, {screenWidth/4, 0}, Fade(WHITE, 0.8f));
            DrawTriangle({screenWidth/4*3+screenWidth/16, 0}, {screenWidth/4*3, 0}, {screenWidth/4*3, screenHeight/8}, Fade(WHITE, 0.8f));
            DrawTextEx(fontCaption, TextFormat(musicStatus_list[curMusicIndex].name.c_str()), {screenWidth/4, 0}, screenHeight/16, 0, BLACK);
            DrawTextEx(fontCaption, TextFormat(musicStatus_list[curMusicIndex].authorName.c_str()), {screenWidth/4, screenHeight/16}, screenHeight/16, 0, GRAY);
        EndDrawing();
    }
    void update() {
        // 播放音乐
        play_repeat(BGMlst[curMusicIndex]);

        bg.nextFrame();
        rotation-=0.5;
        //====================键盘操控=================
        if(isKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }
        if(isKeyPressed(KEY_ENTER)) {
            isEnd = true;
            selectedMusicIndex = curMusicIndex;
            selectedMusicStatus = musicStatus_list[curMusicIndex];
        }
        if(isKeyPressed(KEY_A) || isKeyPressed(KEY_LEFT)) {
            curMusicIndex = (curMusicIndex + BGMlst.size() - 1) % BGMlst.size();
            PlayMusicStream(BGMlst[curMusicIndex]); 
        }
        if(isKeyPressed(KEY_D) || isKeyPressed(KEY_RIGHT)) {
            curMusicIndex = (curMusicIndex + 1) % BGMlst.size();
            PlayMusicStream(BGMlst[curMusicIndex]); 
        }
    }
    SceneType end() {
        // for(auto& music: BGMlst) {
        //     UnloadMusicStream(music);
        // }
        // for(auto& illust: illustLst) {
        //     UnloadTexture(illust);
        // }
        // bg.unload();
        // UnloadFont(fontCaption);
        
        if(selectedMusicIndex != -1) {
            return SCENE_PLAY;
        }

        return SCENE_MAIN;
    }
    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
