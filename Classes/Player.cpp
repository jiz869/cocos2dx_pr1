#include "Player.h"

GPlayer::~GPlayer()
{
    sprite->release();
    playerTexture->release();
    animationRun->release();
}

CCsprite* GPlayer::CreatePlayerSprite()
{
    width=50;
    height=34;
    playerTexture = CCTextureCache::sharedTextureCache()->addImage("sonic.png");
    playerTexture->retain();

    sprite = CCSprite::createWithTexture(this->sonic_texture, CCRectMake(0,0, width, height));
    sprite->retain();
    sprite->setPosition(ccp(100,100));

    //load animation
    animationRun = CCAnimation::create();
    animationRun->retain();
    animationRun->setDelayPerUnit(1.0/8.0);

    for(int i=0; i<8; ++i) {
        CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->sonic_texture, CCRectMake(i*width, 0, width, height));
        animationRun->addSpriteFrame(frame);
    }

    return sprite;
}

