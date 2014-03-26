#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "cocos2d.h"
using namespaces cocos2d;

class GObject : public CCNode {
pulic:
    GObject();
    virtual ~GObject(){
    }
    virtual void Load(char *name) {
    }
    virtual void SetPosition(float x, float y) {
    }
    virtual CCNode* Node() {
        return NULL;
    }
    virtual void Step(float dt) {
    }

};


#endif

