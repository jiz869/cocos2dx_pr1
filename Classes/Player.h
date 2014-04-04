#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

const float JMP_Y_SPEED = 7.0;
const float GRAVITY_Y = -0.4;
const CCPoint INIT_POS = ccp(100, 100);
const float SHORT_JMP_SPEED = 3.0;
const float INIT_GAME_SPEED = 3.0;

class GPlayer : public CCObject
{
    CCTexture2D *playerTexture;
    CCAnimation *animationRun;
    CCAnimation *animationJumpUp;
    CCAnimation *animationJumpDown;
    float animRunRate;

    float width;    // width of single frame in sprite sheet
    float height;
    CCSprite *sprite;

    //physics
    CCPoint velocity;
    CCPoint gravity;
    CCSize designSize;
    bool applyGravity;

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
    void EnableGravity();
    void DisableGravity() {
    	applyGravity = false;
    }
    void UpdateGravity();
    bool IsGravityDown() {
        return (gravity.y < 0.0);
    }
    void Step(float);

    //collision detection
    void GetAABB(CCPoint &o, float &w, float &h);

    //data
    ePlayerState state;
};

#define on_the_air(s) (s == JMP1 || s == JMP2)
#define in_upper (sprite->getPosition().y > designSize.height/2)

#endif

