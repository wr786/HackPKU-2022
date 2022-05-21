#pragma once
#include "raylib.h"
#include "fileio.h"
#include "config.cpp"
#include <string>
#include <cstdio>
#include <vector>
using namespace std;

class MusicStatus {
public:
    string name;
    string authorName;
    MusicStatus() {
        #ifdef DEBUG
            printf("[Warning] Music Status created without arguments\n");
        #endif
    }
    MusicStatus(string _name, string _authorName): name(_name), authorName(_authorName) {}
    MusicStatus(string mname) {
        size_t sepPos = mname.find('-');
        size_t endPos = mname.find_last_of('.');
        if (sepPos == string::npos) {
            name = mname.substr(0, endPos);
            authorName = "Unknown";
        } else {
            authorName = mname.substr(0, sepPos-1);
            name = mname.substr(sepPos+2, endPos-sepPos-2);
        }
    }

    string fullName() const {
        if(authorName != "Unknown")
            return authorName + " - " + name;
        else
            return name;
    }

};
vector<string> music_list;
vector<MusicStatus> musicStatus_list;
vector<Sound> taps;

int init_music_list() {
    if(music_list.size()) 
        return music_list.size();
    getJustCurrentFile(MUSIC_FOLDER, music_list);
    return music_list.size();
}

int init_music_status_list() {
    if(musicStatus_list.size())
        return musicStatus_list.size();
    init_music_list();
    for(auto& mname: music_list) {
        musicStatus_list.push_back(MusicStatus(mname));
    }
    return musicStatus_list.size();
}

void init_taps(){
    if(taps.size()) return;
    for(int i=1; i<=15; i++){
        string tap_filename = SOUND_FOLDER + "Tap 00.wav";
        int endPos = tap_filename.find_last_of('.');
        tap_filename[endPos-2] = '0' + i/10;
        tap_filename[endPos-1] = '0' + i%10;  
        taps.push_back(LoadSound(tap_filename.c_str()));
    }
}

void play_once(Sound &tap){
    PlaySound(tap);
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
