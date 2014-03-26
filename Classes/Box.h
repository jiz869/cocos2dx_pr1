#ifndef _BOX_H
#define _BOX_H
#include "GameObject.h"

// box is a game object which can be regarded as a large box

class GBox : public GObject {
    CCSpriteBatchNode *batchNode;
    CCTexture2D *texture;
    CCPoint velocity;
public:
    virtual void Load(char *name) {
    }

    virtual void SetPosition(float x, float y) {
    }

    virtual CCNode* Node() {
        return (CCNode*)batchNode;
    }

    virtual void Step(float dt) {
    }

    int width;
    int height;
};

#endif;
