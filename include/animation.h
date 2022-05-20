#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"

class Animation {
public:
    Texture2D pic;
    int framesPerRow, rowCounts;
    float frameWidth, frameHeight;
    int curFrame, curRow, frameCounter;
    Rectangle frameRec;
    float fps = 12;
    Animation() {}
    Animation(string path, int framesPerRow, int rowCounts, float _fps) {
        pic = LoadTexture(path.c_str());
        frameWidth = (float)(pic.width / framesPerRow);
        frameHeight = (float)(pic.height / rowCounts);
        fps = _fps;
        curFrame = 0, curRow = 0, frameCounter = 0;
        frameRec = {0, 0, frameWidth, frameHeight};
    }

    void nextFrame() {
        frameCounter++;
        if(frameCounter > 60./fps) {
            curFrame++;
            if(curFrame >= framesPerRow) {
                curFrame = 0;
                curRow++;
                if(curRow >= rowCounts) {
                    curRow = 0;
                }
            }
            frameCounter = 0;
        }
        frameRec.x = frameWidth*curFrame;
        frameRec.y = frameHeight*curRow;
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