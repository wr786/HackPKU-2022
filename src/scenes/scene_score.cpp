#include "raylib.h"
#include "vector"
#include "scene.h"
#include "../config.cpp"
#include <cstdlib>
#include <fstream>
#include <io.h>

extern Player* player;

class SceneScore: public SceneBase{
private:
    string songName;
    Texture2D texture_background;
    Font font_caption;
    Font font_caption2;
    bool isEnd = false;
    int score;
    int total_perfect;
    int total_good;
    int total_miss;
    int combo;
    int max_combo;

public:
    void init() {
        songName = "aria";
        // 这里改成接口获取
        if (player)
        {
            score = player -> score;
            total_perfect = player -> total_perfect;
            total_good = player -> total_good;
            total_miss = player -> total_miss;;
            combo = player -> combo;
            max_combo = player -> max_combo;
        }
        else {
            score = 0;
            total_perfect = 5;
            total_good = 5;
            total_miss = 10;
            combo = 10;
            max_combo = 10;
        }
        font_caption = LoadFontEx("../resource/font/bb2180.ttf", 96, 0, 0);
        font_caption2 = LoadFontEx("../resource/font/Cinzel-Black.ttf", 96, 0, 0);
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
            
            // 曲名信息
            DrawRectangle(1000, 60, 800, 100, Fade(BLACK, 0.5f));
            DrawTriangle({900, 110}, {1000, 160}, {1000, 60}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption, TextFormat("%s", songName.c_str()), {1100, 65}, 90, 0, WHITE);

            // 画历史分数
            if (combo == max_combo)
            {
                DrawTextEx(font_caption, TextFormat("New record !!!"), {900, 220}, 120, 0, YELLOW);
            }

            // 画分数
            DrawRectangle(700, 400, 800, 100, Fade(BLACK, 0.5f));
            DrawTriangle({1500, 400}, {1500, 500}, {1600, 450}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption, TextFormat("Score: %04d", score), {900, 415}, 70, 0, WHITE);

            // 画评级
            DrawTriangle({595, 450}, {805, 450}, {700, 345}, Fade(BLACK, 0.9f));
            DrawTriangle({595, 450}, {700, 555}, {805, 450}, Fade(BLACK, 0.9f));
            DrawTriangle({615, 450}, {785, 450}, {700, 365}, Fade(WHITE, 0.3f));
            DrawTriangle({615, 450}, {700, 535}, {785, 450}, Fade(WHITE, 0.3f));
            if(score <= 20) {
                DrawTextEx(font_caption, TextFormat("D"), {678, 418}, 80, 0, GREEN);
            } else if(score <= 40) {
                DrawTextEx(font_caption, TextFormat("C"), {680, 420}, 80, 0, RED);
            } else if(score <= 60) {
                DrawTextEx(font_caption, TextFormat("B"), {661, 381}, 150, 0, YELLOW);
            } else if(score <= 80) {
                DrawTextEx(font_caption, TextFormat("A"), {658, 372}, 150, 0, ORANGE);
            } else if(score <= 100) {
                DrawTextEx(font_caption, TextFormat("S"), {662, 374}, 150, 0, PURPLE);
            } else if(score <= 120) {
                DrawTextEx(font_caption, TextFormat("V"), {658, 382}, 150, 0, BLUE);
            } else if(score > 120) {
                DrawTextEx(font_caption, TextFormat("P"), {663, 376}, 150, 0, PINK);
            }
            
            // 画各note个数
           float base = 550;
            float interval = 50;
            float basex = 1250;
            float acc = float(total_good + total_perfect)/float(total_miss + total_good + total_perfect) * 100;
            DrawRectangle(1150, 520, 800, 300, Fade(BLACK, 0.5f));
            DrawTriangle({1050, 670}, {1150, 820}, {1150, 520}, Fade(BLACK, 0.5f));
            DrawTextEx(font_caption2, TextFormat("Perfect: %04d", total_perfect ), {basex, base}, 40, 0, PURPLE);
            DrawTextEx(font_caption2, TextFormat("Good: %04d", total_good ), {basex, base + interval}, 40, 0, PURPLE);
            DrawTextEx(font_caption2, TextFormat("Miss: %04d", total_miss ), {basex, base + interval * 2}, 40, 0, PURPLE);
            DrawTextEx(font_caption2, TextFormat("Max combo: %04d", max_combo ), {basex, base + interval * 3}, 40, 0, PURPLE);
            DrawTextEx(font_caption2, TextFormat("ACC: %.2f%%", acc), {basex, base + interval * 4 + 10}, 60, 0, WHITE);

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
        return SCENE_SELECT;
    }

};
