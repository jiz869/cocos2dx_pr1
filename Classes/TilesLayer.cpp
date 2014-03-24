#include "TilesLayer.h"

#include "cocos2d.h"
using namespace cocos2d;

// tiles.png are 32x32
//ground rect y - 224   x from 0,

GTilesLayer::GTilesLayer() : velocity(-3.0, 0.0), batchNode(0), texture(0)
{
}

GTilesLayer::~GTilesLayer()
{
    batchNode->release();
}

//load map from a map file and create corresponding sprites to batchnode
void GTilesLayer::LoadMap()
{
    //load image
    batchNode = CCSpriteBatchNode::create("tiles.png", 30);
    texture = batchNode->getTexture();
    batchNode->retain();

//td: *loading map from xml

    // player 50x34 at position 100,100, achor point 0, 0
    int i=0;
    for(i=0; i<5; ++i) {
        int idx = i%3;
        CCSprite *tile = CCSprite::createWithTexture( texture, CCRectMake(idx*32, 224, 32, 32) );
        tile->setOpacity(100);
        batchNode->addChild(tile);
        tile->setAnchorPoint(ccp(0.0, 0.0));
        tile->setPosition( ccp(32*i, 100-32) );
    }
    rightMostPosition = ccp(32*(i-1), 100-32);
}

void GTilesLayer::SetVelocity(CCPoint v)
{
    velocity = v;
}

void GTilesLayer::Step(float dt)
{
	//CCLog("GTilesLayer step %f", dt);
    CCObject *obj = NULL;
    CCArray *tileArray = batchNode->getChildren();
    CCPoint rmp = ccp(0.0, 100-32);
    CCARRAY_FOREACH(tileArray, obj)
    {
    	CCSprite *tile = (CCSprite*)obj;
        CCPoint pos = tile->getPosition();
        pos = pos + velocity;
        if( pos.x + 32 > 0 ) {
            tile->setPosition( pos );
            if( pos.x > rmp.x )
                rmp = pos;
        }else {
            batchNode->removeChild(tile, true);
        }
    }
    rightMostPosition = rmp;

    int idx = 0;
    while ( rightMostPosition.x < 480 ) {
        //idx = idx%3;
        CCSprite *tile = CCSprite::createWithTexture( texture, CCRectMake((idx%3)*32, 224, 32, 32) );
        tile->setOpacity(100);
        batchNode->addChild(tile);
        tile->setAnchorPoint(ccp(0.0, 0.0));
        tile->setPosition( ccp( rightMostPosition.x + 32, 100-32) );
        CCPoint p = tile->getPosition();
        CCLog("Add new tile at (x: %f, y: %f)", p.x, p.y);
        idx++;
        rightMostPosition.x += 32;
    }
}

