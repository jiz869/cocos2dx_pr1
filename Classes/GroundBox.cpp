#include <string.h>
#include "GroundBox.h"

GGroundBox::GGroundBox()
{
}

GGroundBox::~GGroundBox()
{
    if(batchNode) {
        batchNode->release();
        batchNode = 0;
    }
}

//nc - number of columns
//nr - number of rows
//fn - file name
//tile_xy - vector of x,y coordinates for corresponding tile
//tile_xy should contain corordinates in row based way
// tile_xy[nc*r + c]
void GGroundBox::LoadTiles(int nc, int nr, float w, float h,
        char *fn, vector<CCPoint> &tile_xy, bool flipY)
{
    batchNode = CCSpriteBatchNode::create(fn);
    texture = batchNode->getTexture();
    batchNode->retain();
    batchNode->setAnchorPoint(ccp(0.0, 0.0));

    for(int r=0; r<nr; ++r) {
        for(int c=0; c<nc; ++c) {
            CCPoint xy = tile_xy[nc*r+c];
            CCSprite *tile = CCSprite::createWithTexture( texture, CCRectMake(xy.x, xy.y, w, h) );
            tile->setOpacity(160);
            tile->setAnchorPoint(ccp(0.0, 0.0));
            //relative position to batchNode
            tile->setPosition(ccp(c*w, r*h));
            tile->setFlipY(flipY);
            batchNode->addChild(tile);
        }
    }

    width = nc*w;
    height = nr*h;
    state = OBJ_LOADED;
}

void GGroundBox::LoadStoneGround()
{
    vector<CCPoint> tiles;
    tiles.reserve(3*3);
    for(int r=0; r<3; ++r)
        for(int c=0; c<3; ++c) {
            tiles[3*r+c] = ccp(191, 256);
        }
    LoadTiles(3, 3, 32, 32, "tiles.png", tiles );
    return;
}

void GGroundBox::LoadGrassRock()
{
    vector<CCPoint> tiles;
    tiles.reserve(4);
    tiles[0] = ccp(32*2, 0);
    tiles[1] = ccp(32*4, 0);
    tiles[2] = ccp(32*0, 0);
    tiles[3] = ccp(32*8, 0);
    LoadTiles(4, 1, 32, 32, "tiles2.png", tiles, true);
}

void GGroundBox::LoadTree()
{
    vector<CCPoint> tiles;
    tiles.reserve(1);
    tiles[0] = ccp(26*32, 28*32);
    LoadTiles(1, 1, 64, 64, "tiles.png", tiles, false);
}

void GGroundBox::Load(char *name)
{
    if(strncmp(name, "stone ground", 50) == 0) {
        LoadStoneGround();
    }else if( strncmp(name, "grass rock", 50) == 0) {
        LoadGrassRock();
    }else if( strncmp(name, "tree", 50) == 0 ){
        LoadTree();
    }
    strncpy(objName, name, 50);
}

void GGroundBox::SetObjectPosition(float x, float y)
{
    batchNode->setPosition(ccp(x, y));
}

void GGroundBox::Step(float dt)
{
    CCPoint pos = batchNode->getPosition();
    CCPoint newpos = pos + velocity;
    if(state != OBJ_INACTIVE) {
        batchNode->setPosition( pos + velocity );
        if( newpos.x+width < 0 || newpos.y + height < 0 ) {
            state = OBJ_INACTIVE;
        }
    }
}



