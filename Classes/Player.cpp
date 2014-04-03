#include "Player.h"

GPlayer::GPlayer() : width(0), height(0), state(RUN), sprite(0), applyGravity(false)
{
    designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
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
    applyGravity = false;
    CCLog("set player state RUN");
}

void GPlayer::JumpUp()
{
    if(state == RUN || state == JMP1) {
        sprite->stopAllActions();
        sprite->setTextureRect( CCRectMake(5*width, 1*height+1, width, height) );
        state = (state == JMP1) ? JMP2 : JMP1;
        if( in_upper ) {
            velocity = ccp(0.0, -7.0);
        }else{
            velocity = ccp(0.0, 7.0);
        }
        CCLog("set player state JMPx");
    }
}

void GPlayer::JumpDown()
{
    //sprite->setFlipY(true);
    sprite->stopAllActions();
    sprite->setTextureRect( CCRectMake(6*width, 1*height+1, width, height) );
    if( state == RUN) {
        state = JMP1;
        CCLog("JumpDown: set player state JMP1");
    }
}

void GPlayer::EnableGravity()
{
    applyGravity = true;
}

void GPlayer::UpdateGravity()
{
    //determine gravity direction (now it's based on player's position)
    if( GetPlayerPosition().y <= designSize.height/2 ) {
        gravity = ccp(0.0, -0.4);
        sprite->setFlipY(false);
    }else{
        gravity = ccp(0.0, 0.4);
        sprite->setFlipY(true);
    }
}

void GPlayer::SwitchGravity()
{
   //important: clear velocity
   velocity = ccp(0.0, 0.0);
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
    CCPoint oldVelocity = velocity;
    CCPoint oldPosition = GetPlayerPosition();

    CCPoint pos = sprite->getPosition();
    pos = pos + velocity;
    sprite->setPosition(pos);

    if(applyGravity)
    	velocity = velocity + gravity;

    if( oldVelocity.y == 0.0 && velocity.y != 0.0 && state == RUN) {
        state = JMP1;
        CCLog("set player state JMP1");
    }

    //check and set gravity
    UpdateGravity();

    //velocity and gravity have the same direction

    if( (oldPosition.y <= designSize.height/2 && pos.y > designSize.height/2) ||
        (oldPosition.y > designSize.height/2 && pos.y <= designSize.height/2) ) {
        SwitchGravity();
        JumpDown();
        CCLog("flip gravity!");
    }else if(oldVelocity.y * velocity.y <= 0.0 && velocity.y * gravity.y > 0.0) {
    	CCLog("player's old velocity.y %f new velocity.y %f)", oldVelocity.y, velocity.y);
        JumpDown();
    }

    //CCLog("player step position(%f, %f) velocity(%f, %f) gravity(%f, %f) state %d",
    //		pos.x, pos.y, velocity.x, velocity.y, gravity.x, gravity.y, state);
}


