#ifndef _TILES_H_
#define _TILES_H_

#include "cocos2d.h"
using namespace cocos2d;

class GTiles : public CCObject {
    CCSpriteBatchNode *batchNode;

public:
    GTiles();
    ~GTiles();
    CCSpriteBatchNode* GetBatchNode();

};

#endif

