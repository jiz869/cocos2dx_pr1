#ifndef _TILES_H_
#define _TILES_H_

#include "cocos2d.h"
using namespace cocos2d;

class GTilesLayer : public CCObject {
    CCSpriteBatchNode *batchNode;

public:
    GTilesLayer();
    ~GTilesLayer();
    inline CCSpriteBatchNode* GetBatchNode() {
    	return batchNode;
    }

};

#endif

