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

public:
    enum ePlayerState {
        RUN,
        JMP1,
        JMP2,
    };
    GPlayer();
    ~GPlayer();

    CCSprite* CreatePlayerSprite();

    //move/control
    void SetPlayerPosition(float x, float y);

    CCPoint GetPlayerPosition() {
    	return sprite->getPosition();
    }

    CCPoint GetPlayerVelocity() {
        return velocity;
    }
    void SetPlayerVelocity(float x, float y) {
        velocity = ccp(x, y);
    }

    void JumpUp();
    void JumpDown();
    void Run();
    void SwitchGravity();
    void EnableGravity(float x, float y) {
        gravity = ccp(x, y);
    }
    void Step(float);

    //collision detection
    void GetAABB(CCPoint &o, float &w, float &h);

    //data
    ePlayerState state;
};
#define on_the_air(s) (s == JMP1 || s == JMP2)

#endif

