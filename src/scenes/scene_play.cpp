#include "raylib.h"
#include "scene.h"
#include "../config.cpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#define PLAYER_X 200
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define NOTE_WIDTH 20
#define NOTE_HEIGHT 20
#define NOTE_SPEED 6
#define NOTE_OFFSET 100
#define SCORE_PER_BLOCK 100
#define MAX_NUM_MUSIC 1
#define TIME_OFFSET 4
#define RAIL_OFFSET 500
#define RAIL_DISTANCE 200

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
};

class Notes{
public:
    int id;
    float time;
    int rail;
    Color color;
    Rectangle bounds;
    int is_valid() { return bounds.x > 0 && (bounds.x + bounds.width) < GetScreenWidth(); }
};

class Song{
public:
    vector<Notes> notes;
    // used for store valid notes (inside the window)
    map<int, Notes> valid_notes;
    vector<float> notes_created;
    // 先考虑一歌一谱
    string note_file_name = selectedMusicStatus.fullName() + ".txt";
    // used for create mode
    string note_created_file_name = "test_created.txt";
    string music_file_name = selectedMusicStatus.fullName() + ".wav";
    Music back_sound;

    void InitMusic() {
        string path = MUSIC_FOLDER;
        path += music_file_name;
        printf("[debug] music_file_name %s\n", path.c_str());
        back_sound = LoadMusicStream(path.c_str());
    }

    void CreateNotesFromFile() {
        string path = NOTES_FOLDER;
        path += note_file_name;
        // read from note file and create notes
        ifstream infile(path);
        printf("[debug] note_file_name %s\n", path.c_str());
        float time;
        int id = 0;
        while(!infile.eof()) {
            id += 1;
            infile >> time;

            Notes note;
            note.id = id;
            note.time = time;
            note.color = BLACK;
            note.rail = rand()%2;
            note.bounds = (Rectangle){ NOTE_OFFSET + PLAYER_X + note.time * 60 * NOTE_SPEED, float(note.rail * RAIL_DISTANCE + RAIL_OFFSET), NOTE_WIDTH, NOTE_HEIGHT };
            notes.push_back(note);
        }
        notes.pop_back();
    }

    void SaveNotesToFile() {
        string path = NOTES_FOLDER;
        path += note_created_file_name;
        ofstream outfile(path);
        for (const auto &e : notes_created) 
            outfile << e << "\n";
    }

    float compute_score(double time) {
        float score = 0.f;
        double min_dis = 99.0;
        for(auto iter = valid_notes.begin(); iter != valid_notes.end(); iter++) {
            min_dis = abs(time - iter->second.time) > min_dis ? min_dis : abs(time - iter->second.time);
        }
        printf("[debug] min_dis %f\n", min_dis);
        if ((min_dis - TIME_OFFSET) < 0.5f)
            score = 3.f;
        else if ((min_dis - TIME_OFFSET) > 0.5f && (min_dis - TIME_OFFSET) < 1.0f)
            score = 2.f;
        else if ((min_dis - TIME_OFFSET) > 1.0f && (min_dis - TIME_OFFSET) < 2.0f)
            score = 1.f;
        return score;
    }
};


class ScenePlay: public SceneBase {

private:
    bool isEnd = false;
    Player *player;
    Song *song;
    float score = 0.f;

    bool loaded = false;
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;
    
public:
    void init() {
        printf("[debug] calling ScenePlay");

        SetTargetFPS(60);
        InitAudioDevice();

        player = new Player();
        player->rail = 1;
        player->color = RED;
        player->bounds = (Rectangle){ PLAYER_X, float(player->rail * RAIL_DISTANCE + RAIL_OFFSET), PLAYER_WIDTH, PLAYER_HEIGHT };

        song = new Song();
        song->CreateNotesFromFile();
        song->InitMusic();

        if(!loaded) {
            background = LoadTexture(string(IMAGE_FOLDER+"cyberpunk_street_background.png").c_str());
            midground = LoadTexture(string(IMAGE_FOLDER+"cyberpunk_street_midground.png").c_str());
            foreground = LoadTexture(string(IMAGE_FOLDER+"cyberpunk_street_foreground.png").c_str());

            loaded = true;
        }
        
        scrollingBack = 0.0f;
        scrollingMid = 0.0f;
        scrollingFore = 0.0f;
    }

    void draw() {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 5.0f, WHITE);
            DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 5.0f, WHITE);
            // Draw midground image twice
            DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 5.0f, WHITE);
            DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 5.0f, WHITE);
            // Draw foreground image twice
            DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 5.0f, WHITE);
            DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 5.0f, WHITE);



            DrawText(TextFormat("%f", score), 20, 20, 40, GRAY);
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
        if(IsKeyPressed(KEY_Q)) {
            isEnd = true;
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_F)) {
            player->rail -= 1;
            printf("[debug] press key Time: %f\n", GetTime());
            score += song->compute_score(GetTime());
        }
        if (IsKeyPressed(KEY_J) || IsKeyPressed(KEY_K)) {
            player->rail += 1;
            score += song->compute_score(GetTime());
        }

        // Check player not out of rails
        if (player->rail > 1) 
            player->rail = 1;
        else if (player->rail < 0) 
            player->rail = 0;

        player->bounds = (Rectangle){ PLAYER_X, float(player->rail * RAIL_DISTANCE + RAIL_OFFSET), PLAYER_WIDTH, PLAYER_HEIGHT };

        for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
            iter->bounds.x -= NOTE_SPEED;
            if (iter->is_valid()) {
                song->valid_notes.insert(pair<int, Notes>(iter->id, *iter));
            }
            else {
                song->valid_notes.erase(iter->id);
            }
        }

        // 背景
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;
        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -background.width*2) scrollingBack = 0;
        if (scrollingMid <= -midground.width*2) scrollingMid = 0;
        if (scrollingFore <= -foreground.width*2) scrollingFore = 0;

        //====================create mode=================
        /*
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_F) || IsKeyDown(KEY_J) || IsKeyDown(KEY_K))
            song->notes_created.push_back(GetTime());

        if (IsKeyDown(KEY_S))
            song->SaveNotesToFile();
        */
    }

    SceneType end() {
        return SCENE_SELECT;
    }

    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
