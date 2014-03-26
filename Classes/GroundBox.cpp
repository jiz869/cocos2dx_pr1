#include <string.h>
#include "GroundBox.h"

GGroundBox::GGroundBox()
{
}

GGroundBox::~GGroundBox()
{
}

void GGroundBox::Load(char *name)
{
    if(strncmp(name, "stone ground", 50) == 0) {
        LoadStoneGround();
    }
}


