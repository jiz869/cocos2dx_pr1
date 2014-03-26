#ifndef _GROUND_BOX_H
#define _GROUND_BOX_H

#include "Box.h"

//all objects as ground
//they can be contacted from top not left
class GGroundBox : public GBox {
    void LoadStoneGround();

public:
    GGroundBox();
    ~GGroundBox();

    virtual void Load(char *name);
    virtual void SetObjectPosition(float x, float y);
    virtual void Step(float dt);

};

#endif

