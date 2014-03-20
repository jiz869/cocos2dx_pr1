#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

class GPlayer 
{
    enum ePlayerState {
        RUN;
    };

    CCTexture2D *playerTexture;
    CCAnimation *animationRun;
    CCAnimation *animationJumpUp;
    float width;
    float height;
    CCSprite *sprite;

public:
    GPlayer() : width(0), height(0), state(RUN), sprite(0)
    {
    };

    ~GPlayer();

    CCSprite* CreatePlayerSprite();

    //sonic move/control
    void JumpUp(); 
    void JumpDown();
    void Run();
    void JumpUpDone();
    void JumpDownDone();

    //data
    ePlayerState state;
};

#endif

