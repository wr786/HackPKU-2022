#include "raylib.h"
#include "scene.h"
#include "../config.cpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#define PLAYER_X 200
#define PLAYER_WIDTH 120
#define PLAYER_HEIGHT 120
#define NOTE_WIDTH 120
#define NOTE_HEIGHT 100
#define NOTE_SPEED 16
#define NOTE_OFFSET 100
#define SCORE_PER_BLOCK 100
#define MAX_NUM_MUSIC 1
#define TIME_OFFSET 4
#define RAIL_OFFSET 500
#define RAIL_DISTANCE 200

enum PlayerStatus {
    RUNNING = 0,
    KICKING_UP,
    KICKING_DOWN,
    PLAYER_STATUS_TOTAL
};

enum NoteStatus {
    SPAWN = 0,
    PERFECT = 1,
    GOOD = 2,
    MISS = 3,
    NOTE_STATUS_TOTAL
};

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
    PlayerStatus status = RUNNING;
};

class Notes{
public:
    int id;
    float time;
    int rail;
    Color color;
    Rectangle bounds;
    NoteStatus status = SPAWN;
    bool is_miss() 
    {
        if (bounds.x < PLAYER_X)
            return true;
        else
            return false;
    }
    bool is_valid() 
    {
        if (bounds.x > 0 && (bounds.x + bounds.width) < GetScreenWidth())
            return true;
        else
            return false;
    }
};

class Song{
public:
    vector<Notes> notes;
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
            infile >> time;

            Notes note;
            note.id = id;
            note.time = time;
            note.color = BLACK;
            note.rail = rand() % 2;
            note.bounds = (Rectangle){ NOTE_OFFSET + PLAYER_X + note.time * 60 * NOTE_SPEED, float(note.rail * RAIL_DISTANCE + RAIL_OFFSET), NOTE_WIDTH, NOTE_HEIGHT };
            notes.push_back(note);
            id += 1;
        }
        notes.pop_back();
        for (auto i : notes)
        {
            printf("[debug] notes %d x: %f\n", i.id, i.bounds.x);
        }
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
        int index = 0;
        for (auto iter = notes.begin(); iter != notes.end(); iter++) {
            min_dis = abs(time - iter->time) > min_dis ? min_dis : abs(time - iter->time);
            index = abs(time - iter->time) > min_dis ? index : abs(time - iter->id);
        }
        printf("[debug] min_dis %f\n", min_dis);
        if (min_dis < 0.2f)
        {
            score = 3.f;
            notes[index].status = PERFECT;
        }
        else if (min_dis > 0.5f && min_dis < 2.0f)
        {
            score = 1.f;
            notes[index].status = GOOD;
        }
        else
        {
            score = 0.f;
            notes[index].status = MISS;
        }
        return score;
    }
};


class ScenePlay: public SceneBase {

private:
    bool isEnd = false;
    bool gotoScore = false;
    Player *player;
    Song *song;

    float InitTime = 0.f;
    float score = 0.f;

    bool loaded = false;
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;
    Animation playerRunning;
    Animation playerUpKicking;
    Animation playerDownKicking;
    Texture2D textureNote;
    
    bool isKeyPressed(KeyboardKey key) {
        if(IsKeyPressed(key)) {
            play_once(taps[14]);
            return true;
        }
        return false;
    }
public:

    float compute_score() {
        float score = 0.f;
        double min_dis = 999999.0;
        int index = 0;
        for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
            printf("[debug] notes %d x: %f\n", iter->id, iter->bounds.x);
            min_dis = (abs(PLAYER_X - iter->bounds.x) > min_dis) ? min_dis : abs(PLAYER_X - iter->bounds.x);
            index = (abs(PLAYER_X - iter->bounds.x) > min_dis) ? index : abs(iter->id);
        }
        printf("[debug] min_dis %f\n", min_dis);
        printf("[debug] notes %d has min_dis %f\n", index, min_dis);
        if (min_dis < 100.0f)
        {
            score = 3.f;
            song->notes[index].status = PERFECT;
            printf("[debug] notes %d status %d \n", index, song->notes[index].status );
        }
        else if (min_dis > 100.f && min_dis < 200.0f)
        {
            score = 1.f;
            song->notes[index].status = GOOD;
            printf("[debug] notes %d status %d \n", index, song->notes[index].status );
        }
        else
        {
            score = 0.f;
            // song->notes[index].status = MISS;
        }
        return score;
    }
    void init() {
        printf("[debug] calling ScenePlay");

        SetTargetFPS(60);
        InitAudioDevice();

        InitTime = GetTime();

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
            playerRunning = Animation(IMAGE_FOLDER+"player_running.png", 6, 1, 20);
            playerUpKicking = Animation(IMAGE_FOLDER+"player_upkicking.png", 9, 1, 20);
            playerDownKicking = Animation(IMAGE_FOLDER+"player_downkicking.png", 8, 1, 20);
            textureNote = LoadTexture(string(IMAGE_FOLDER+"soccer.png").c_str());

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

            DrawText(TextFormat("%d", score), 20, 20, 40, GRAY);

            // player
            // DrawRectangle(player->bounds.x, player->bounds.y, PLAYER_WIDTH, PLAYER_HEIGHT, player->color);
            if(player->status == RUNNING) {
                DrawTexturePro(playerRunning.getTexture(), playerRunning.getFrame(), {player->bounds.x, player->bounds.y, 200, 120}, {0.f, 0.f}, 0, WHITE);
            } else if (player->status == KICKING_UP) {
                DrawTexturePro(playerUpKicking.getTexture(), playerUpKicking.getFrame(), {player->bounds.x, player->bounds.y, 120, 200}, {0.f, 0.f}, 0, WHITE);
            } else if (player->status == KICKING_DOWN) {
                DrawTexturePro(playerDownKicking.getTexture(), playerDownKicking.getFrame(), {player->bounds.x, player->bounds.y, 200, 120}, {0.f, 0.f}, 0, WHITE);
            }

            for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
                if (iter->status == SPAWN)
                {
                    DrawTexturePro(textureNote, {0, 0, (float)textureNote.width, (float)textureNote.height}, iter->bounds, {0.f, 0.f}, 0, WHITE);
                }
                else if (iter->status == MISS)
                {
                    DrawTexturePro(textureNote, {0, 0, (float)textureNote.width, (float)textureNote.height}, iter->bounds, {0.f, 0.f}, 0, GRAY);
                }      
                else
                {
                    DrawRectangle(iter->bounds.x, iter->bounds.y, iter->bounds.width, iter->bounds.height, iter->color);
                }          
                // DrawRectangle(iter->bounds.x, iter->bounds.y, iter->bounds.width, iter->bounds.height, iter->color);
            }
        EndDrawing();
    }

    void update() {
        PlayMusicStream(song->back_sound);
        UpdateMusicStream(song->back_sound);
        if(player->status == RUNNING) {
            playerRunning.nextFrame();
        } else if (player->status == KICKING_UP) {
            if(!playerUpKicking.nextFrame()) {
                player->status = RUNNING;
                player->rail = 1;   // 下落
            }
        } else if (player->status == KICKING_DOWN) {
            if(!playerDownKicking.nextFrame()) {
                player->status = RUNNING;
            }
        }

        //====================键盘操控=================
        if(IsKeyPressed(KEY_ESCAPE)) {
            isEnd = true;
        }
        if(IsKeyPressed(KEY_Q)) {
            isEnd = true;
        }
        if (isKeyPressed(KEY_D) || isKeyPressed(KEY_F)) {
            player->rail = 0;
            player->status = KICKING_UP;
            playerUpKicking.curFrame = 0;
            // printf("[debug] press key Time: %f\n", GetTime() - InitTime);
            // score += song->compute_score(GetTime() - InitTime);
            score += compute_score();
        }
        if (isKeyPressed(KEY_J) || isKeyPressed(KEY_K)) {
            player->rail = 1;
            player->status = KICKING_DOWN;
            playerDownKicking.curFrame = 0;
            // printf("[debug] press key Time: %f\n", GetTime() - InitTime);
            // score += song->compute_score(GetTime() - InitTime);
            score += compute_score();
        }

        // Check player not out of rails
        if (player->rail > 1) 
            player->rail = 1;
        else if (player->rail < 0) 
            player->rail = 0;

        player->bounds = (Rectangle){ PLAYER_X, float(player->rail * RAIL_DISTANCE + RAIL_OFFSET), PLAYER_WIDTH, PLAYER_HEIGHT };

        for (auto iter = song->notes.begin(); iter != song->notes.end(); iter++) {
            iter->bounds.x -= NOTE_SPEED;
            if (iter->is_miss() && iter->status != PERFECT && iter->status != GOOD)
            {
                iter->status = MISS;  
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

        // check if music end
        if (GetMusicTimePlayed(song->back_sound) >= GetMusicTimeLength(song->back_sound) - 0.1)
        {
            isEnd = true;
            gotoScore = true;
        }

    }

    SceneType end() {
        if(gotoScore) {
            gotoScore = false;
            return SCENE_SCORE;
        }
        return SCENE_SELECT;
    }

    bool is_end() {
        if(!isEnd) return false;
        isEnd = false;
        return true;
    }
};
