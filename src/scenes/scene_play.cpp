#include "raylib.h"
#include "vector"
#include "scene.h"
#include "config.h"
#include <cstdlib>
#include <fstream>
#define PLAYER_X 200
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define NOTE_WIDTH 40
#define NOTE_HEIGHT 40
#define MAX_NUM_MUSIC 1

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
};

class Notes{
public:
    int rail;
    Color color;
    Rectangle bounds;
};

class Song{
public:
    vector<Notes> notes;
    string note_file_name = "test_.txt";
    string music_file_name = "test.wav";
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
        string path = MUSIC_FOLDER;
        path += note_file_name;
        // read from note file and create notes
        ifstream infile(path);
        printf("[debug] note_file_name %s\n", path.c_str());
        float x;
        while(!infile.eof())
        {
            infile >> x;

            Notes note;
            note.color = BLACK;
            note.rail = rand()%2;
            note.bounds.x = x;
            note.bounds =  (Rectangle){ PLAYER_X + note.bounds.x, note.rail * 120 + 200, NOTE_WIDTH, NOTE_HEIGHT };
            notes.push_back(note);
        }
        notes.pop_back();
    }
};


class ScenePlay: public SceneBase {

private:
    bool isEnd = false;
    Player *player;
    Song *song;
    
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
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_F)) 
            player->rail -= 1;
        if (IsKeyDown(KEY_J) || IsKeyDown(KEY_K)) 
            player->rail += 1;

        // Check player not out of rails
        if (player->rail > 1) 
            player->rail = 1;
        else if (player->rail < 0) 
            player->rail = 0;

        player->bounds = (Rectangle){ PLAYER_X, player->rail * 120 + 200, PLAYER_WIDTH, PLAYER_HEIGHT };

        for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
            iter->bounds.x -= 4.15;
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