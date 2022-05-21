#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "raylib.h"

enum PlayerStatus {
    RUNNING = 0,
    KICKING_UP,
    KICKING_DOWN,
    PLAYER_STATUS_TOTAL
};

class Player{
public:
    int rail;
    Color color;
    Rectangle bounds;
    PlayerStatus status = RUNNING;
    int score = 0;
    int total_perfect = 0;
    int total_good = 0;
    int total_miss = 0;
    int combo = 0;
    int max_combo = 0;
};

#endif