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
        songName = "sand";

        font_caption = LoadFontEx("../resource/font/bb2180.ttf", 96, 0, 0);
        string path = IMAGE_FOLDER + songName + ".png";
        texture_background = LoadTexture("../resource/image/bg_scenescore.png");
        score = 0;
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
            // // // // 中心边框
            // DrawRectangle(0, 250, 1600, 400, Fade(BLACK, 0.6f));
            // DrawRectangle(0, 350, 1600, 200, Fade(WHITE, 0.4f));
            // // // // 光栅化 
            // // DrawTriangle({500, 320}, {370, 450}, {630, 450}, Fade(WHITE, 1.f));
            // // DrawTriangle({500, 580}, {630, 450}, {370, 450}, Fade(WHITE, 1.f));
            // // DrawTriangle({500, 330}, {380, 450}, {620, 450}, Fade(BLACK, 1.f));
            // // DrawTriangle({500, 570}, {620, 450}, {380, 450}, Fade(BLACK, 1.f));

            // 画分数
            DrawRectangle(700, 400, 800, 100, Fade(BLACK, 0.5f));
            DrawTriangle({1500, 400}, {1500, 500}, {1600, 450}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption, TextFormat("%08d", score), {900, 415}, 70, 0, WHITE);

            // 画评级
            DrawTriangle({595, 450}, {805, 450}, {700, 345}, Fade(BLACK, 0.9f));
            DrawTriangle({595, 450}, {700, 555}, {805, 450}, Fade(BLACK, 0.9f));
            DrawTriangle({615, 450}, {785, 450}, {700, 365}, Fade(WHITE, 0.3f));
            DrawTriangle({615, 450}, {700, 535}, {785, 450}, Fade(WHITE, 0.3f));
            if(score <= 5000000) {
                DrawTextEx(font_caption, TextFormat("D"), {680, 420}, 80, 0, GREEN);
            } else if(score <= 7000000) {
                DrawTextEx(font_caption, TextFormat("C"), {680, 420}, 80, 0, RED);
            } else if(score <= 8000000) {
                DrawTextEx(font_caption, TextFormat("B"), {680, 420}, 150, 0, YELLOW);
            } else if(score <= 9000000) {
                DrawTextEx(font_caption, TextFormat("A"), {680, 420}, 150, 0, ORANGE);
            } else if(score <= 9500000) {
                DrawTextEx(font_caption, TextFormat("S"), {680, 420}, 150, 0, PURPLE);
            } else if(score < 10000000) {
                DrawTextEx(font_caption, TextFormat("V"), {680, 420}, 150, 0, BLUE);
            } else if(score == 10000000) {
                DrawTextEx(font_caption, TextFormat("P"), {680, 420}, 150, 0, PINK);
            }

            // // // 画ACC
            // // DrawTextEx(font_caption, TextFormat("ACC:%.2f%%", scoreboard.get_acc()), {207, 433}, 30, 0, WHITE);
            // // // 画Combo //todo full combo特殊字幕
            // // DrawTextEx(font_caption, TextFormat("Max Combo: %d", scoreboard.maxCombo), {1060, 433}, 40, 0, WHITE);
            // // // 画各note个数
            // // DrawTextEx(font_caption, TextFormat("PURE"), {700, 610}, 30, 0, PINK);
            // // DrawTextEx(font_caption, TextFormat("FAR"), {900, 610}, 30, 0, ORANGE);
            // // DrawTextEx(font_caption, TextFormat("LOST"), {1100, 610}, 30, 0, GRAY);
            // // DrawTextEx(font_caption, TextFormat("%3d", scoreboard.pure), {700, 565}, 40, 0, WHITE);
            // // DrawTextEx(font_caption, TextFormat("%3d", scoreboard.far), {888, 565}, 40, 0, WHITE);
            // // DrawTextEx(font_caption, TextFormat("%3d", scoreboard.lost), {1095, 565}, 40, 0, WHITE);

            // // // 画历史分
            // // if(!createScore) {  // 如果
            // //     if(scoreboard.get_score() >= curSelectedHighscore)
            // //         DrawTextEx(font_caption, TextFormat("NEW BEST"), {660, 280}, 40, 0, GREEN);
            // //     DrawTextEx(font_caption, TextFormat("%08d",curSelectedHighscore), {960, 280}, 40, 0, WHITE);
            // //     string fmt = scoreboard.get_score() >= curSelectedHighscore? "+%8d" : "-%8d";
            // //     DrawTextEx(font_caption, TextFormat(fmt.c_str(), abs(scoreboard.get_score() - curSelectedHighscore)), {1200, 280}, 40, 0, WHITE);
            // // }
            // // 画字幕
            // // if(createScore) {
            // DrawTextEx(font_caption, TextFormat("CREATE MODE CREATE MODE CREATE MODE CREATE MODE CREATE MODE"), {0, 270}, 60, 0, PINK);
            // DrawTextEx(font_caption, TextFormat("ATE MODE CREATE MODE CREATE MODE CREATE MODE CREATE MODE"), {0, 570}, 60, 0, PINK);
            // // }

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
