#include "Box.h"

GBox::GBox() : batchNode(0), texture(0)
{
}

GBox::~GBox()
{
}

void GBox::GetAABB(CCPoint &o, float &w, float &h)
{
    o = batchNode->getPosition();
    w = width;
    h = height;
}


