#include "raylib.h"
#include "scene.h"
#include "../config.cpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#define PLAYER_X 200
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define NOTE_WIDTH 20
#define NOTE_HEIGHT 20
#define NOTE_SPEED 6
#define NOTE_OFFSET 100
#define SCORE_PER_BLOCK 100
#define MAX_NUM_MUSIC 1

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
};

class Notes{
public:
    float time;
    int rail;
    Color color;
    Rectangle bounds;
};

class Song{
public:
    vector<Notes> notes;
    vector<float> notes_created;
    // 先考虑一歌一谱
    string note_file_name = selectedMusicStatus.name + ".txt";
    // used for create mode
    string note_created_file_name = "test_created.txt";
    string music_file_name = selectedMusicStatus.name + ".wav";
    Music back_sound;

    void InitMusic()
    {
        string path = MUSIC_FOLDER;
        path += music_file_name;
        printf("[debug] music_file_name %s\n", path.c_str());
        back_sound = LoadMusicStream(path.c_str());
    }

    void CreateNotesFromFile()
    {
        string path = NOTES_FOLDER;
        path += note_file_name;
        // read from note file and create notes
        ifstream infile(path);
        printf("[debug] note_file_name %s\n", path.c_str());
        float time;
        while(!infile.eof())
        {
            infile >> time;

            Notes note;
            note.time = time;
            note.color = BLACK;
            note.rail = rand()%2;
            note.bounds = (Rectangle){ NOTE_OFFSET + PLAYER_X + note.time * 60 * NOTE_SPEED, note.rail * 120 + 200, NOTE_WIDTH, NOTE_HEIGHT };
            notes.push_back(note);
        }
        notes.pop_back();
    }

    void SaveNotesToFile()
    {
        string path = NOTES_FOLDER;
        path += note_created_file_name;
        ofstream outfile(path);
        for (const auto &e : notes_created) 
            outfile << e << "\n";
    }
};


class ScenePlay: public SceneBase {

private:
    bool isEnd = false;
    Player *player;
    Song *song;
    float totel_score = 0;
    float last_score = 0;
    int last_level = -1;
    int level(float x) {
        if(x < 60) return 0;
        if(x < 70) return 1;
        if(x < 85) return 2;
        return 3;
    }
    
public:
    void init() {
        printf("[debug] calling ScenePlay");

        SetTargetFPS(60);
        InitAudioDevice();

        player = new Player();
        player->rail = 0;
        player->color = RED;
        player->bounds = (Rectangle){ PLAYER_X, player->rail * 120 + 200, PLAYER_WIDTH, PLAYER_HEIGHT };

        song = new Song();
        song->CreateNotesFromFile();
        song->InitMusic();
    }

    void draw() {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(player->bounds.x, player->bounds.y, PLAYER_WIDTH, PLAYER_HEIGHT, player->color);
            for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
                DrawRectangle(iter->bounds.x, iter->bounds.y, iter->bounds.width, iter->bounds.height, iter->color);
            }
        EndDrawing();
    }

    void update() {
        PlayMusicStream(song->back_sound);
        UpdateMusicStream(song->back_sound);
        //====================键盘操控=================
        if(IsKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }
        int pre_rail = player->rail;
        bool flag = false;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_F)) {
            player->rail -= 1;
            flag = true;
        }
        if (IsKeyDown(KEY_J) || IsKeyDown(KEY_K)) {
            player->rail += 1;
            flag = true;
        }

        // Check player not out of rails
        if (player->rail > 1) 
            player->rail = 1;
        else if (player->rail < 0) 
            player->rail = 0;

        player->bounds = (Rectangle){ PLAYER_X, player->rail * 120 + 200, PLAYER_WIDTH, PLAYER_HEIGHT };

        for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
            iter->bounds.x -= NOTE_SPEED;
        }

        //====================create mode=================
        /*
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_F) || IsKeyDown(KEY_J) || IsKeyDown(KEY_K))
            song->notes_created.push_back(GetTime());

        if (IsKeyDown(KEY_S))
            song->SaveNotesToFile();
        */
        // 记录score
        float new_score = 0;
        int new_level = -1;
        if(((pre_rail ^ player->rail) != 0 && pre_rail != 0) || (pre_rail == 0 && flag)
            || ((pre_rail ^ player->rail) != 0 && pre_rail != 1) || (pre_rail == 1 && flag)) {
            for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
                float temp = iter->bounds.x;
                if(temp + NOTE_WIDTH < PLAYER_X) continue;
                if(temp - NOTE_WIDTH > PLAYER_X) break;
                if(iter->bounds.y != player->bounds.y) continue;
                new_score = SCORE_PER_BLOCK - 2 * abs((iter->bounds.x) - PLAYER_X);
                new_level = level(new_score);
                last_score = new_score;
                totel_score += new_score;
                last_level = new_level;
            }
        }
    }

    SceneType end() {
        return SCENE_NULL;
    }

    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
