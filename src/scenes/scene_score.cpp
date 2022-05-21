#include "raylib.h"
#include "vector"
#include "scene.h"
#include "../config.cpp"
#include <cstdlib>
#include <fstream>
#include <io.h>

class SceneScore: public SceneBase{
private:
    string songName;
    Texture2D texture_background;
    Font font_caption;
    int score = 0;
    bool isEnd = false;

public:
    void init() {
        songName = "aria";

        font_caption = LoadFontEx("../resource/font/bb2180.ttf", 96, 0, 0);
        string path = IMAGE_FOLDER + songName + ".png";
        texture_background = LoadTexture("../resource/image/bg_scenescore.png");
        SetTargetFPS(60);
    }

    void update() {
        // do nothing.
    }
    void draw() {
        //todo 可能最好有个待机音乐？
        BeginDrawing();
            ClearBackground(WHITE);
            DrawTextureEx(texture_background, { 0, 0}, 0.0f, screenWidth / (float)texture_background.width, WHITE);
            DrawTriangle({500, 900}, {900, 900}, {900, 0}, Fade(BLACK, 0.3f));
            DrawTriangle({500, 0}, {900, 900}, {900, 0}, Fade(BLACK, 0.3f));
            DrawRectangle(900, 0, 900, 900, Fade(BLACK, 0.3f));
            DrawRectangle(900, 0, 900, 900, Fade(BLACK, 0.3f));
            
            // // // 曲名信息
            DrawRectangle(1000, 60, 800, 100, Fade(BLACK, 0.5f));
            DrawTriangle({900, 110}, {1000, 160}, {1000, 60}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption, TextFormat("%s", songName.c_str()), {1200, 65}, 90, 0, WHITE);

            // 画分数
            DrawRectangle(700, 400, 800, 100, Fade(BLACK, 0.5f));
            DrawTriangle({1500, 400}, {1500, 500}, {1600, 450}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption, TextFormat("%08d", score), {900, 415}, 70, 0, WHITE);

            // 画评级
            DrawTriangle({595, 450}, {805, 450}, {700, 345}, Fade(BLACK, 0.9f));
            DrawTriangle({595, 450}, {700, 555}, {805, 450}, Fade(BLACK, 0.9f));
            DrawTriangle({615, 450}, {785, 450}, {700, 365}, Fade(WHITE, 0.3f));
            DrawTriangle({615, 450}, {700, 535}, {785, 450}, Fade(WHITE, 0.3f));
            if(score <= 20) {
                DrawTextEx(font_caption, TextFormat("D"), {680, 420}, 80, 0, GREEN);
            } else if(score <= 40) {
                DrawTextEx(font_caption, TextFormat("C"), {680, 420}, 80, 0, RED);
            } else if(score <= 60) {
                DrawTextEx(font_caption, TextFormat("B"), {680, 420}, 150, 0, YELLOW);
            } else if(score <= 80) {
                DrawTextEx(font_caption, TextFormat("A"), {680, 420}, 150, 0, ORANGE);
            } else if(score <= 100) {
                DrawTextEx(font_caption, TextFormat("S"), {680, 420}, 150, 0, PURPLE);
            } else if(score < 120) {
                DrawTextEx(font_caption, TextFormat("V"), {680, 420}, 150, 0, BLUE);
            } else if(score >= 140) {
                DrawTextEx(font_caption, TextFormat("P"), {680, 420}, 150, 0, PINK);
            }

        EndDrawing();

        if(IsKeyPressed(KEY_Q)) {isEnd = true;}
    }
    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
    SceneType end() {
        UnloadTexture(texture_background);
        UnloadFont(font_caption);
        return SCENE_SCORE;
    }

};
