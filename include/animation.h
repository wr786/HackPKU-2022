#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"
#include <cassert>
#include <iostream>
using namespace std;

class Animation {
public:
    Texture2D pic;
    int framesPerRow, rowCounts;
    float frameWidth, frameHeight;
    int curFrame=0, curRow=0, frameCounter;
    Rectangle frameRec;
    float fps = 12;
    Animation() {}
    Animation(string path, int _framesPerRow, int _rowCounts, float _fps) {
        pic = LoadTexture(path.c_str());
        framesPerRow = _framesPerRow;
        rowCounts = _rowCounts;
        frameWidth = (float)(pic.width / _framesPerRow);
        frameHeight = (float)(pic.height / _rowCounts);
        fps = _fps;
        curFrame = 0, curRow = 0, frameCounter = 0;
        frameRec = {0, 0, frameWidth, frameHeight};
    }

    bool nextFrame() {
        bool ret = true;
        frameCounter++;
        if(frameCounter > 60./fps) {
            curFrame++;
            if(curFrame >= framesPerRow) {
                curFrame = 0;
                curRow++;
                if(curRow >= rowCounts) {
                    curRow = 0;
                    ret = false;
                }
            }
            frameCounter = 0;
        }
        assert(curFrame < framesPerRow);
        assert(curRow < rowCounts);
        frameRec.x = frameWidth*curFrame;
        frameRec.y = frameHeight*curRow;
        return ret;
    }

    const Rectangle& getFrame() const {
        return frameRec;
    }

    const Texture2D& getTexture() const {
        return pic;
    }

    void unload() {
        UnloadTexture(pic);
    }
};

#endif