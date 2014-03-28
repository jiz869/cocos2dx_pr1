#include <algorithm>
#include "GameWorld.h"
#include "AnimationScene.h"

using namespace cocos2d;
static bool PointInSprite(CCPoint &p, CCSprite &sprite);

static int getRandom(int low, int high)
{
	if ( low - high < 0x10000L )
	      return low + ( ( random() >> 8 ) % ( high + 1 - low ) );

	return low + ( random() % ( high + 1 - low ) );
}

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

}

void GameWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
	player.JumpUp();
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

#define AddStoneGround(x, y_offset) \
{ \
    gbox =  new GGroundBox(); \
    gbox->Load("stone ground"); \
    gbox->SetObjectPosition((x), 100 - gbox->height + (y_offset)); \
    gbox->SetVelocity(ccp(-2, 0)); \
    mapObjects.push_back(gbox); \
    this->addChild(gbox->Node()); \
}


void GameWorld::InitMap()
{
    GGroundBox *gbox;

    AddStoneGround(100, 0);
    AddStoneGround(250, 0);
    AddStoneGround(400, 30);
    AddStoneGround(550, -30);
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
    	float y_offset = (float)getRandom(-30, 30);
        if(first_obj->state != OBJ_INACTIVE) {
            CCLog("No invalid object available!");
        }else{
            first_obj->SetObjectPosition(pos.x+w+80, 100-first_obj->height + y_offset);
            first_obj->state = OBJ_ACTIVE;
        }
    }
}

#define segment_overlap(l1, r1, l2, r2) \
    ( (((l1) < (r2)) && ((l1) > (l2))) || \
      (((r1) < (r2)) && ((r1) > (l2))) || \
      (((r2) < (r1)) && ((r2) > (l1))) || \
      (((l2) < (r1)) && ((l2) > (l1))) )


void GameWorld::PhysicsStep(float dt)
{
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);
    CCPoint player_rb(player_pos.x + player_w - 1,
                      player_pos.y);
    CCPoint player_lb(player_pos.x, player_pos.y);

    //check walk intersect with player
    unsigned int n = mapObjects.size();
    GObject *on_obj=0;
    CCPoint pos;
    float w, h;
    for(int i=0; i<n; ++i) {
        GObject *obj = mapObjects[i];
        obj->GetAABB(pos, w, h);

        //bottom test
        //check x
        if( segment_overlap(player_lb.x+11, player_rb.x-12, pos.x+1, pos.x+w-2 ) ) {
            //check y
            if( segment_overlap(player_pos.y-4, player_pos.y+player_h, pos.y, pos.y+h-1) ) {
                //intersects
                on_obj = obj;
                /*
                CCLog("player_lb (%f, %f) player_rb(%f, %f)",
                        player_lb.x, player_lb.y, player_rb.x, player_rb.y);
                CCLog("intersect with object (%f, %f) width %f height %f",
                        pos.x, pos.y, w, h);
                        */
                break;
            }
        }

        //right test
        if( segment_overlap(player_pos.y-6, player_pos.y+player_h, pos.y, pos.y+h-1) ) {
            if( segment_overlap(player_lb.x+15, player_rb.x-20, pos.x+1, pos.x+w-2 ) ) {
                CCLog("bumped into wall");
            }
        }
    }

    if(on_obj) {
        if(player.state != GPlayer::RUN) {
            CCLog("intersect at a solid tile. set player run");
            player.Run();
            player.SetPlayerPosition(player_pos.x, pos.y+h);
        }
    }else{
        //player is on the air
        if( player.state != GPlayer::JMP_DOWN ) {
            CCLog("player on the air. set player jump down");
            player.JumpDown();
        }
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
    player.Step(dt);
    PhysicsStep(dt);
}



