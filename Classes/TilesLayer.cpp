#include "TilesLayer.h"

#include "cocos2d.h"
using namespace cocos2d;

// tiles.png are 32x32
//ground rect y - 224   x from 0,

GTilesLayer::GTilesLayer()
{
    //load image
    batchNode = CCSpriteBatchNode::create("tiles.png", 30);
    batchNode->retain();
}

GTilesLayer::~GTilesLayer()
{
    batchNode->release();
}


CCSpriteBatchNode* GTilesLayer::GetBatchNode()
{
	return 0;
}

