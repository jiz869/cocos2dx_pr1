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
}

void GPlayer::JumpUp()
{
    float jmpduration = 0.3;
    sprite->stopAllActions();
    sprite->setTextureRect( CCRectMake(5*width, 1*height+1, width, height) );
    CCFiniteTimeAction *movUp = CCMoveBy::create( jmpduration, ccp(0,100) );
    CCEaseOut *emovUp = CCEaseOut::create( (CCActionInterval*)movUp, 2 );
    CCFiniteTimeAction *movUpDone = CCCallFuncN::create( this, callfuncN_selector(GPlayer::JumpUpDone) );
    sprite->runAction( CCSequence::create(emovUp, movUpDone, NULL) );
    state = JMP_UP;
}

void GPlayer::JumpDown()
{
    float jmpduration = 0.35;
    //this->sonic->setFlipY(true);
    sprite->stopAllActions();
    sprite->setTextureRect( CCRectMake(6*width, 1*height+1, width, height) );
    //CCFiniteTimeAction *movDown = CCMoveBy::create( jmpduration, ccp(0,-100) );
    CCFiniteTimeAction *movDown = CCMoveTo::create( jmpduration, ccp(100, 100) );
    CCEaseIn *emovDown = CCEaseIn::create( (CCActionInterval*)movDown, 2 );
    CCFiniteTimeAction *movDownDone = CCCallFuncN::create( this, callfuncN_selector(GPlayer::JumpDownDone) );
    sprite->runAction( CCSequence::create(emovDown, movDownDone, NULL) );
    state = JMP_DOWN;
}

void GPlayer::JumpUpDone()
{
    JumpDown();
}

void GPlayer::JumpDownDone()
{
    Run();
}

