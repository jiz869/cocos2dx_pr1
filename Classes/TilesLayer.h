#ifndef _TILES_H_
#define _TILES_H_

#include "cocos2d.h"
using namespace cocos2d;

class GTilesLayer : public CCNode {
    CCSpriteBatchNode *batchNode;
    CCTexture2D *texture;
    CCPoint velocity;

    //tiles composition
    CCPoint rightMostPosition;

public:
    GTilesLayer();
    virtual ~GTilesLayer();
    virtual void LoadMap();
    inline CCNode* BatchNode() {
    	return (CCNode*)batchNode;
    }

    void SetVelocity(CCPoint v);
    void Step(float dt);
};

#endif

