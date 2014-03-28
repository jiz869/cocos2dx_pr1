#include <algorithm>
#include "GameWorld.h"
#include "AnimationScene.h"

using namespace cocos2d;
static bool PointInSprite(CCPoint &p, CCSprite &sprite);

GameWorld::~GameWorld()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this

	//release all mapobjects
    unsigned int n = mapObjects.size();
    for(int i=0; i<n; ++i) {
       delete mapObjects[i];
    }
}

GameWorld::GameWorld()
{

}

CCScene* GameWorld::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameWorld *layer = GameWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameWorld::init()
{
	bool bRet = false;
	do
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(GameWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();

		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to GameWorld layer as a child layer.
		this->addChild(pMenu, 0.1);

		///////////////////////////////////////////////
		// 2. add your codes below...
        ///////////////////////////////////////////////
		this->setTouchEnabled(true);
        this->schedule( schedule_selector(GameWorld::step) );

        this->addChild( player.CreatePlayerSprite() );
        player.Run();

        //mapLayer.LoadMap();
        //this->addChild( mapLayer.BatchNode());

        InitMap();

        bRet = true;
	} while (0);

	return bRet;
}

void GameWorld::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    //CCScene *pScene = AnimationScene::scene();
    //CCDirector::sharedDirector()->pushScene(pScene);
    CCDirector::sharedDirector()->popScene();
#endif
}

// cpp with cocos2d-x
void GameWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);
    player.JumpUp();
}

void GameWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

}

void GameWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

}

static bool PointInSprite(CCPoint &p, CCSprite &sprite)
{
    float sx = sprite.getPositionX();
    float sy = sprite.getPositionY();
    float w = sprite.getContentSize().width;
    float h = sprite.getContentSize().height;
    float l = sx - w/2;
    float r = sx + w/2;
    float b = sy - h/2;
    float t = sy + h/2;

    if( (p.x > l) && (p.x < r) && (p.y < t) && (p.y > b) )
        return true;
    return false;
}

#define AddStoneGround(x) \
{ \
    gbox =  new GGroundBox(); \
    gbox->Load("stone ground"); \
    gbox->SetObjectPosition((x), 100 - gbox->height); \
    gbox->SetVelocity(ccp(-1, 0)); \
    mapObjects.push_back(gbox); \
    this->addChild(gbox->Node()); \
}


void GameWorld::InitMap()
{
    GGroundBox *gbox =  new GGroundBox();

    gbox->Load("stone ground");
    gbox->SetObjectPosition(200, 100 - gbox->height);
    gbox->SetVelocity(ccp(-1, 0));
    mapObjects.push_back(gbox);

    this->addChild(gbox->Node());

    gbox =  new GGroundBox();

    gbox->Load("stone ground");
    gbox->SetObjectPosition(80, 100 - gbox->height);
    gbox->SetVelocity(ccp(-1, 0));
    mapObjects.push_back(gbox);

    this->addChild(gbox->Node());

    gbox =  new GGroundBox();

    gbox->Load("stone ground");
    gbox->SetObjectPosition(320, 100 - gbox->height);
    gbox->SetVelocity(ccp(-1, 0));
    mapObjects.push_back(gbox);

    this->addChild(gbox->Node());

    AddStoneGround(450);
    AddStoneGround(600);
}

//debug
void GameWorld::dump_mapObjects()
{
    unsigned int n = mapObjects.size();
    for(int i=0; i<n; ++i) {
        CCPoint o;
        float w, h;
        mapObjects[i]->GetAABB(o, w, h);
        CCLog("mapObjects[%d] at (%f, %f) width %f height %f",
                i, o.x, o.y, w, h);
    }
}

static bool CompareX2(GObject* a, GObject* b)
{
    CCPoint pos_a, pos_b;
    float wa, wb;
    float ha, hb;
    a->GetAABB( pos_a, wa, hb);
    b->GetAABB( pos_b, wb, hb);

    return ( (pos_a.x+wa) < (pos_b.x+wb) );
}

void GameWorld::RenewMap()
{
    //sort it based on right most x cooridinate
    sort( mapObjects.begin(), mapObjects.end(), CompareX2 );
    GObject *last_obj = mapObjects.back();
    GObject *first_obj = mapObjects.front();

    CCPoint pos;
    float w, h;

    //check last object
    last_obj->GetAABB(pos, w, h);

    //to do: add logic to generate new map objects
    if(pos.x+w < designSize.width) {
        if(first_obj->state != OBJ_INACTIVE) {
            CCLog("No invalid object available!");
        }else{
            first_obj->SetObjectPosition(pos.x+w+32, 100-first_obj->height);
            first_obj->state = OBJ_ACTIVE;
        }
    }
}

void GameWorld::PhysicsStep(float dt)
{
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);

    unsigned int n = mapObjects.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = mapObjects[i];
        
    }
}

void GameWorld::step(float dt)
{
	//mapLayer.Step(dt);
    //dump_mapObjects();

    unsigned int n = mapObjects.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = mapObjects[i];
        obj->Step(dt);
    }
    RenewMap();
    PhysicsStep(float dt);
}



