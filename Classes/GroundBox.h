#ifndef _GROUND_BOX_H
#define _GROUND_BOX_H

#include <vector>
#include "Box.h"

using namespace std;
//all objects as ground
//they can be contacted from top not left
class GGroundBox : public GBox {
    void LoadStoneGround();
    void LoadGrassRock();
    void LoadTree();
    void LoadFlyingStone();
    //general tiles loading function
    //nc - number of columns
    //nr - number of rows
    //fn - file name
    //tile_xy - vector of x,y coordinates for corresponding tile
    //tile_xy should contain corordinates in row based way
    // tile_xy[nc*r + c]
    void LoadTiles(int nc, int nr, float w, float h,
            char *fn, vector<CCPoint> &tile_xy, bool flipY=false);

public:
    GGroundBox();
    ~GGroundBox();

    virtual void Load(char *name);
    virtual void SetObjectPosition(float x, float y);
    virtual void Step(float dt);
    virtual void GetAABB(CCPoint &, float &w, float &h);

};

#endif

