#include "Player.h"

GPlayer::GPlayer() : width(0), height(0), state(RUN), sprite(0)
{
}

GPlayer::~GPlayer()
{
    sprite->release();
    playerTexture->release();
    animationRun->release();
}

CCSprite* GPlayer::CreatePlayerSprite()
{
    width=50;
    height=34;
    playerTexture = CCTextureCache::sharedTextureCache()->addImage("sonic.png");
    playerTexture->retain();

    sprite = CCSprite::createWithTexture(this->playerTexture, CCRectMake(0,0, width, height));
    sprite->retain();
    sprite->setAnchorPoint(ccp(0,0));
    sprite->setPosition(ccp(100,100));

    //load animation
    animationRun = CCAnimation::create();
    animationRun->retain();
    animationRun->setDelayPerUnit(1.0/8.0);

    for(int i=0; i<8; ++i) {
        CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(i*width, 0, width, height));
        animationRun->addSpriteFrame(frame);
    }

    return sprite;
}

void GPlayer::Run()
{
    sprite->stopAllActions();
    CCAnimate *aa = CCAnimate::create(animationRun);
    CCRepeatForever *rep = CCRepeatForever::create(aa);
    sprite->runAction(rep);
    state = RUN;
    velocity = ccp(0.0, 0.0);
    gravity = ccp(0.0, 0.0);
}

void GPlayer::JumpUp()
{
    if(state == RUN || state == JMP1) {
        sprite->stopAllActions();
        sprite->setTextureRect( CCRectMake(5*width, 1*height+1, width, height) );
        state = (state == JMP1) ? JMP2 : JMP1;
        velocity = ccp(0.0, 7.0);
        //gravity = ccp(0.0, -1.0);
    }
}

void GPlayer::JumpDown()
{
    //sprite->setFlipY(true);
    sprite->stopAllActions();
    sprite->setTextureRect( CCRectMake(6*width, 1*height+1, width, height) );
    if( state == RUN) {
        state = JMP1;
    }
}

void GPlayer::SwitchGravity()
{
   gravity = ccp( gravity.x, -gravity.y); 
   if( gravity.y > 0 ) {
       sprite->setFlipY(true);
   }
}

void GPlayer::GetAABB(CCPoint &o, float &w, float &h)
{
    o = sprite->getPosition();
    w = width;
    h = height;
}

void GPlayer::SetPlayerPosition(float x, float y)
{
    sprite->setPosition(ccp(x,y));
}

void GPlayer::Step(float dt)
{
    CCPoint pos = sprite->getPosition();
    pos = pos + velocity;
    sprite->setPosition(pos);

    velocity = velocity + gravity;
    if(velocity.y < 0.0) {
    	CCLog("player's velocity.y < 0.0 position (%f, %f)", pos.x, pos.y);
        JumpDown();
    }else if(velocity.y > 0.0 && state == RUN) {
        state = JMP1;
    }

    //CCLog("player step position(%f, %f) velocity(%f, %f) gravity(%f, %f) state %d",
    //		pos.x, pos.y, velocity.x, velocity.y, gravity.x, gravity.y, state);
}




