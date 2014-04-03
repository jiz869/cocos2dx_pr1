#ifndef __GameWorld_H__
#define __GameWorld_H__

#include <vector>
#include "cocos2d.h"
#include "Player.h"
#include "TilesLayer.h"
#include "GroundBox.h"

using namespace cocos2d;
using namespace std;

struct InterSectionResult{
    bool intersect;
};
//////////////////////////////////////////////////////////////////
class GameWorld : public cocos2d::CCLayerColor
{
    enum tTouchState{
        TS_NONE,
        TS_TOUCHED
    };

public:
	GameWorld();
	~GameWorld();

    void step(float dt);

	// Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(GameWorld);

    //implement base virtual functions
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    //data
    CCSize designSize;

protected:
    GPlayer player;
    float speed;    //player's moving speed
    float distance;

    //map data and control
    vector<GObject*> bottomObjects;
    vector<GObject*> upperObjects;
    vector<GObject*> obstacles;
   

    GObject* CreateObstacle(char *name);
    GObject* GetObstacle(char *name);
    void InitMap();
    void RenewMap();

    void PhysicsStep(float);

    //collistion test
    bool SideTest(GObject *obj);
    bool TopSideTest(GObject *obj);
    bool BottomTest(GObject *obj);
    bool TopTest(GObject *obj);

    //        t
    //        |
    // l------|------r
    //        |
    //        b
    bool SegmentsTest(CCPoint b, CCPoint t, CCPoint l, CCPoint r);

    //scene switch
    void GameOver();

    //debug
    void dump_bottomObjects();
};

#define set_objects_velocity(objs, v) \
{\
    for(int i=0; i<(objs).size(); ++i) { \
        (objs)[i]->SetVelocity(v);\
    }\
}

#endif

