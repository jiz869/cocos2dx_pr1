#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

class GPlayer : public CCObject
{

    CCTexture2D *playerTexture;
    CCAnimation *animationRun;
    CCAnimation *animationJumpUp;
    CCAnimation *animationJumpDown;
    float width;    // width of single frame in sprite sheet
    float height;
    CCSprite *sprite;

    //physics
    CCPoint velocity;
    CCPoint gravity;

    void JumpUpDone();
    void JumpDownDone();

public:
    enum ePlayerState {
        RUN,
        JMP_UP,
        JMP_DOWN
    };
    GPlayer();
    ~GPlayer();

    CCSprite* CreatePlayerSprite();

    //move/control
    void SetPlayerPosition(float x, float y);
    void JumpUp();
    void JumpDown();
    void Run();
    void Step(float);
    
    //collision detection
    void GetAABB(CCPoint &o, float &w, float &h);

    //data
    ePlayerState state;
};
#define on_the_air(s) (s == JMP_UP || s == JMP_DOWN)

#endif

