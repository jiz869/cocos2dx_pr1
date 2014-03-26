#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "cocos2d.h"
using namespace cocos2d;

class GObject : public CCNode {
public:
    GObject() {
    }

    virtual ~GObject(){
    }

    virtual void Load(char *name) {
    }

    virtual void SetObjectPosition(float x, float y) {
    }

    virtual CCNode* Node() {
        return NULL;
    }

    virtual void Step(float dt) {
    }

};


#endif

