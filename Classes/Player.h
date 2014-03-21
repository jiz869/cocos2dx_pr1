#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

class GPlayer : public CCObject
{
    enum ePlayerState {
        RUN,
        JMP_UP,
        JMP_DOWN
    };

    CCTexture2D *playerTexture;
    CCAnimation *animationRun;
    CCAnimation *animationJumpUp;
    CCAnimation *animationJumpDown;
    float width;    // width of single frame in sprite sheet
    float height;
    CCSprite *sprite;

    void JumpUpDone();
    void JumpDownDone();

public:
    GPlayer();
    ~GPlayer();

    CCSprite* CreatePlayerSprite();

    //move/control
    void JumpUp();
    void JumpDown();
    void Run();

    //data
    ePlayerState state;
};

#endif

