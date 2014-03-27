#ifndef _BOX_H
#define _BOX_H
#include "GameObject.h"

// box is a game object which can be regarded as a large box
// position at left/bottom corner

class GBox : public GObject {
protected:
    CCSpriteBatchNode *batchNode;
    CCTexture2D *texture;
    CCPoint velocity;
public:
    GBox();
    ~GBox();
    virtual void Load(char *name) {
    }

    virtual void SetObjectPosition(float x, float y) {
    }

    virtual CCNode* Node() {
        return (CCNode*)batchNode;
    }

    virtual void Step(float dt) {
    }

    virtual void SetVelocity( CCPoint v) {
    	velocity = v;
    }

    virtual void GetAABB(CCPoint &, float &w, float &h);


};

#endif;
