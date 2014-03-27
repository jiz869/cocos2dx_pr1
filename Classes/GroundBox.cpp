#include <string.h>
#include "GroundBox.h"

GGroundBox::GGroundBox()
{
}

GGroundBox::~GGroundBox()
{
    if(batchNode) {
        batchNode->release();
        batchNode = 0;
    }
}

void GGroundBox::LoadStoneGround()
{
    batchNode = CCSpriteBatchNode::create("tiles.png");
    texture = batchNode->getTexture();
    batchNode->retain();
    batchNode->setAnchorPoint(ccp(0.0, 0.0));

    // 32x32 tile
    // 3x3 boxes
    // |xxx
    // |xxx
    // |xxx__
    for(int c=0; c<3; ++c)
        for(int r=0; r<3; ++r) {
            CCSprite *tile = CCSprite::createWithTexture( texture, CCRectMake(384, 321, 32, 32) );
            tile->setOpacity(100);
            tile->setAnchorPoint(ccp(0.0, 0.0));
            //relative position to batchNode
            tile->setPosition(ccp(c*32, r*32));
            batchNode->addChild(tile);
        }

    width = 3*32;
    height = 3*32;
    state = OBJ_LOADED;
}

void GGroundBox::Load(char *name)
{
    if(strncmp(name, "stone ground", 50) == 0) {
        LoadStoneGround();
    }
}

void GGroundBox::SetObjectPosition(float x, float y)
{
    batchNode->setPosition(ccp(x, y));
}

void GGroundBox::Step(float dt)
{
    CCPoint pos = batchNode->getPosition();
    CCPoint newpos = pos + velocity;
    batchNode->setPosition( pos + velocity );
    if( newpos.x+width < 0 || newpos.y + height < 0 ) {
        state = OBJ_INACTIVE;
    }
}



