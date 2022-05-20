#include "raylib.h"
#include "fileio.h"
#include "config.cpp"
#include <string>
#include <vector>
using namespace std;

class MUSIC_STATUS {
public:
    string name;
    string authorName;
};
vector<string> music_list;

int init_music_list() {
    music_list.clear();
    getJustCurrentFile(MUSIC_FOLDER, music_list);
    return music_list.size();
}

bool IF_BGM_PLAYING = false;
void init_BGM_play() {
    IF_BGM_PLAYING = false;
}

void play_repeat(Music &music){
    if (IF_BGM_PLAYING == false){
        PlayMusicStream(music);
        IF_BGM_PLAYING = true;
    }
    else{
        UpdateMusicStream(music);
    }
}
void play_stop(Music & music){
    if (IF_BGM_PLAYING == false){
        printf("Error:the music you stopped hasn't been played!!!\n");
    }
    else{
        StopMusicStream(music);
        IF_BGM_PLAYING = false;
    }
}
