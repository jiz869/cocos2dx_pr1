#include <algorithm>
#include "GameWorld.h"
#include "GameOverScene.h"
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

	//release all map Objects
    unsigned int n = bottomObjects.size();
    for(int i=0; i<n; ++i) {
       delete bottomObjects[i];
    }
    n = upperObjects.size();
    for(int i=0; i<n; ++i) {
       delete upperObjects[i];
    }
    n = obstacles.size();
    for(int i=0; i<n; ++i) {
       delete obstacles[i];
    }

}

GameWorld::GameWorld() : distance(0.0), difficulty(0)
{

}

void GameWorld::InitLevel()
{
    levels[0].minDistance = 0; levels[0].difficulty = 0; levels[0].speed = 3;
    levels[1].minDistance = 2*designSize.width; levels[1].difficulty = 1; levels[1].speed = 3.5;
    levels[2].minDistance = 4*designSize.width; levels[2].difficulty = 2; levels[2].speed = 4;
    levels[3].minDistance = 8*designSize.width; levels[3].difficulty = 3; levels[3].speed = 5;
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
		designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();

        //add background first
        CCSprite *bg = CCSprite::create("meadow.jpg");
        //bg->setPosition(ccp(designSize.width/2, designSize.height/2));
        //float xscale = designSize.width / bg->getContentSize().width;
        //float yscale = designSize.height / bg->getContentSize().height;
        //bg->setScaleX(xscale);
        //bg->setScaleY(yscale);
        this->addChild( bg, -0.1 );

#if 0
		//Add a menu item with "X" image, which is clicked to quit the program.
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

		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to GameWorld layer as a child layer.
		this->addChild(pMenu, 2);
#endif

		this->setTouchEnabled(true);
        this->schedule( schedule_selector(GameWorld::step) );

        //add player
        this->addChild( player.CreatePlayerSprite(), 0.1 );
        player.SetPlayerPosition(80, 100);
        player.Run();

        //add map
        speed = INIT_GAME_SPEED;
        InitMap();

        InitLevel();
        EstimatePhysics();

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

	//CCLog("++++++++after  x:%f, y:%f", location.x, location.y);
    CCPoint v = player.GetPlayerVelocity();
    if(v.y > SHORT_JMP_SPEED) {
       player.SetPlayerVelocity(0, SHORT_JMP_SPEED);
    }else if(v.y < -3) {
       player.SetPlayerVelocity(0, -SHORT_JMP_SPEED);
    }
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

GObject* GameWorld::CreateObstacle(char *name)
{
    GGroundBox *gbox;
    gbox =  new GGroundBox();
    gbox->Load(name);
    obstacles.push_back(gbox);
    this->addChild(gbox->Node());
    gbox->state = OBJ_ACTIVE;
    return gbox;
}

GObject* GameWorld::GetObstacle(char *name)
{
    if(obstacles.size() > 0) {
        for(int i=0; i < obstacles.size(); ++i) {
            if(obstacles[i]->state == OBJ_INACTIVE) {
            	obstacles[i]->state = OBJ_ACTIVE;
                return obstacles[i];
            }
        }
    }

    //need to create a new obstacle object
    return CreateObstacle(name);
}

GObject* GameWorld::GetObject(vector<GObject*> &objs, char *name)
{
    if(objs.size() > 0) {
        for(int i=0; i < objs.size(); ++i) {
            if(objs[i]->state == OBJ_INACTIVE && (strncmp(objs[i]->objName, name, 50) == 0) ) {
            	objs[i]->state = OBJ_ACTIVE;
                return objs[i];
            }
        }
    }

    CCLog("creat a new %s object", name);
    GGroundBox *gbox;
    gbox =  new GGroundBox();
    gbox->Load(name);
    objs.push_back(gbox);
    this->addChild(gbox->Node());
    gbox->state = OBJ_ACTIVE;
    return gbox;
}

#define AddStoneGround(x, y_offset) \
{ \
    gbox =  new GGroundBox(); \
    gbox->Load("stone ground"); \
    gbox->SetObjectPosition((x),50 - gbox->height + (y_offset)); \
    bottomObjects.push_back(gbox); \
    this->addChild(gbox->Node()); \
}

#define AddUpperBox(x) \
{ \
    upBox =  new GGroundBox(); \
    upBox->Load("grass rock"); \
    upBox->SetObjectPosition((x), designSize.height-40 ); \
    upperObjects.push_back(upBox); \
    this->addChild(upBox->Node()); \
}

void GameWorld::InitMap()
{
    GGroundBox *gbox;
    GGroundBox *upBox;

    AddStoneGround(100, 0);
    AddStoneGround(196, 0);
    AddStoneGround(350, 10);
    AddStoneGround(470, -10);
    AddStoneGround(600, 0);
    AddStoneGround(800, 0);

    AddUpperBox(100);
    AddUpperBox(100+upBox->width);
    AddUpperBox(100+2*upBox->width);
    AddUpperBox(100+3*upBox->width);
    AddUpperBox(100+4*upBox->width);
    AddUpperBox(100+5*upBox->width);

    CCPoint v = ccp( -speed, 0);
    set_objects_velocity(bottomObjects, v);
    set_objects_velocity(upperObjects, v);
}

//debug
void GameWorld::dump_bottomObjects()
{
    unsigned int n = bottomObjects.size();
    for(int i=0; i<n; ++i) {
        CCPoint o;
        float w, h;
        bottomObjects[i]->GetAABB(o, w, h);
        CCLog("bottomObjects[%d] at (%f, %f) width %f height %f",
                i, o.x, o.y, w, h);
    }
}


void GameWorld::EstimatePhysics()
{
    //t in time unit ( frame time)
    int t_up = (int)(JMP_Y_SPEED / -GRAVITY_Y) + 1;

    CCPoint v = ccp(0, JMP_Y_SPEED);
    CCPoint s_up = ccp(0.0, 0.0);
    for(int t=0; t<t_up; ++t) {
        s_up = s_up + v;
        v = v + ccp(0, GRAVITY_Y);
    }
    //s_up.y now is the maxmum distance player can jump
    maxJmp1Height = s_up.y;

    CCPoint s_down = ccp(0.0, 0.0);
    v = ccp(0.0, 0.0);
    int t_down = 0;
    while(s_down.y > -s_up.y) {
        s_down = s_down + v;
        v = v + ccp(0, GRAVITY_Y);
        t_down++;
    }

    //now t_down is the time needed to jump down from the highest point
    maxJmp1Distance = (t_up + t_down) * speed;

    //flip jump estimation
    CCPoint s_up2 = ccp(0.0, designSize.height/2 - 32 - s_up.y);
    CCPoint s = ccp(0.0, 0.0);
    int t2=0;
    v = ccp(0.0, JMP_Y_SPEED);
    while(s.y < s_up2.y) {
        s = s + v;
        v = v + ccp(0, GRAVITY_Y);
        t2++;
    }
    maxFlipJmpDistance = (t_up + t_down + t2)*speed;
}

void GameWorld::AddZigZag(GObject *bottomObj, GObject *upperObj)
{
    //------   ZigZag
    float x=0;

    //bottom object AABB
    CCPoint bpos;
    float bw, bh;
    bottomObj->GetAABB(bpos, bw, bh);

    //upper object AABB
    CCPoint upos;
    float uw, uh;
    upperObj->GetAABB(upos, uw, uh);

    //add new bottom object
    float x_gap = maxJmp1Distance;
    GObject *obj = GetObject(bottomObjects, "stone ground");
    obj->SetObjectPosition(bpos.x+bw+x_gap, 32*2-obj->height);
    obj->SetVelocity( ccp(-speed, 0) );
    x = bpos.x+bw+x_gap+obj->width;

    //upper one
    x_gap = maxFlipJmpDistance;
    obj = GetObject(upperObjects, "grass rock");
    obj->SetObjectPosition(x+x_gap, designSize.height-64);
    obj->SetVelocity( ccp(-speed, 0) );
    x = x+x_gap+obj->width;

    //another bottom one
    x_gap = maxFlipJmpDistance;
    obj = GetObject(bottomObjects, "stone ground");
    obj->SetObjectPosition(x+x_gap, 32*2-obj->height);
    obj->SetVelocity( ccp(-speed, 0) );
    x = x + x_gap + obj->width;
}

void GameWorld::AddSequentialJmp2(GObject *bottomObj, GObject *upperObj)
{
    float x=0;
    //bottom object AABB
    CCPoint bpos;
    float bw, bh;
    bottomObj->GetAABB(bpos, bw, bh);

    //upper object AABB
    CCPoint upos;
    float uw, uh;
    upperObj->GetAABB(upos, uw, uh);

    //add new bottom object
    int n = getRandom(4, 6);
    float x_gap = 2*maxJmp1Distance;
    GObject *obj;
    x = bpos.x+bw+x_gap;
    for(int i=0; i<n; ++i) {
        obj = GetObject(bottomObjects, "stone ground");
        obj->SetObjectPosition(x, 32*2-obj->height);
        obj->SetVelocity( ccp(-speed, 0) );
        x = x+x_gap+obj->width;
    }

    //add a upper to make sure no upper obj showed up in this area
    obj = GetObject(upperObjects, "grass rock");
    obj->SetObjectPosition(x-20, designSize.height-64);
    obj->SetVelocity( ccp(-speed, 0) );
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
    //speed control
    int idx=0;
    for(int i=0; i < NUM_LEVELS; ++i) {
        if(distance >= levels[i].minDistance)
            idx = i;
    }
    if( speed != levels[idx].speed ) {
        speed = levels[idx].speed;
        CCPoint v = ccp(-speed, 0);
        set_objects_velocity(bottomObjects, v);
        set_objects_velocity(upperObjects, v);
        //set_objects_velocity(obstacles, v);
        EstimatePhysics();
    }

    //sort it based on right most x cooridinate
    sort( bottomObjects.begin(), bottomObjects.end(), CompareX2 );
    sort( upperObjects.begin(), upperObjects.end(), CompareX2 );
    if( obstacles.size() > 0)
        sort( obstacles.begin(), obstacles.end(), CompareX2 );

    GObject *last_bottom_obj = bottomObjects.back();

    //bottom object
    CCPoint bpos;
    float bw, bh;
    last_bottom_obj->GetAABB(bpos, bw, bh);

    //upper object
    GObject *last_upper_obj = upperObjects.back();
    CCPoint upos;
    float uw, uh;
    last_upper_obj->GetAABB(upos, uw, uh);

    //to do: add logic to generate new map objects
#if 0
    if(bpos.x+bw < designSize.width) {
        float y_offset = (float)getRandom(-10, 10);
        //float x_gap = (float)getRandom(50, 300);
        float x_gap = 1.1*maxJmp1Distance;

        //add new bottom object
        GObject *obj = GetObject(bottomObjects, "stone ground");
        //first_obj->SetObjectPosition(pos.x+w+x_gap, 50-first_obj->height + y_offset);
        obj->SetObjectPosition(bpos.x+bw+x_gap, 32-obj->height);
        obj->SetVelocity( ccp(-speed, 0) );

        //add a tree
        if( x_gap < 200) {
            //GObject *tree = GetObstacle("tree");
            GObject *tree = GetObject(obstacles, "tree");
            float groundX = bpos.x+bw+x_gap;
            //float groundY = 50+y_offset;
            float groundY = 32;
            tree->SetObjectPosition( groundX+20, groundY );
            tree->SetVelocity(ccp(-speed, 0));
        }
    }


    //to do: add logic to generate new map objects
    if(upos.x+uw < designSize.width) {
    	float y_offset = (float)getRandom(-10, 10);
    	float x_gap = (float)getRandom(50, 150);
        GObject *obj = GetObject(upperObjects, "grass rock");
        obj->SetObjectPosition(upos.x+uw+x_gap, designSize.height-40);
        obj->SetVelocity( ccp(-speed, 0) );
    }
#endif
    if(upos.x+uw < designSize.width) {
        int rn = getRandom(1, 10);

        if( rn > 5) {
            AddZigZag(last_bottom_obj, last_upper_obj);
        }else {
            AddSequentialJmp2(last_bottom_obj, last_upper_obj);
        }

        float h = getRandom(80, 160);
        GObject *obj = GetObject(obstacles, "flying stone");
        obj->SetObjectPosition(upos.x+uw+maxJmp1Distance, h);
        float s = getRandom(1, 5);
        obj->SetVelocity( ccp(-speed-s, 0) );
    }

}

bool GameWorld::SideTest(GObject *obj)
{
    //collision test of player's right side and object's left side
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x, pos.y+h-3);
    CCPoint p1(player_pos.x+5, player_pos.y+15);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+15);

    //if speed is very fast, tunneling could happen
    if( SegmentsTest(o1, o2, p1, p2) ) {
        return true;
    }

    return false;
}

bool GameWorld::TopSideTest(GObject *obj)
{
    //collision test of player's right side and object's left side
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x, pos.y+h-3);
    CCPoint p1(player_pos.x+5, player_pos.y+player_h-10);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+player_h-10);

    //if speed is very fast, tunneling could happen
    if( SegmentsTest(o1, o2, p1, p2) ) {
        return true;
    }

    return false;
}

bool GameWorld::BottomTest(GObject *obj)
{
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint p1(player_pos.x+player_w-12, player_pos.y-4);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+player_h-15);
    CCPoint o1(pos.x, pos.y+h-1);
    CCPoint o2(pos.x+w-1, pos.y+h-1);

    if( SegmentsTest(p1, p2, o1, o2) ) {
        return true;
    }

    //left bottom sensor segment
    p1 = ccp(player_pos.x+12, player_pos.y-4);
    p2 = ccp(player_pos.x+12, player_pos.y+player_h-15);

    if( SegmentsTest(p1, p2, o1, o2) ) {
        return true;
    }

    return false;
}

bool GameWorld::TopTest(GObject *obj)
{
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint p1(player_pos.x+player_w-12, player_pos.y+player_h-20);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+player_h+4);
    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x+w-1, pos.y);

    if( SegmentsTest(p1, p2, o1, o2) ) {
        return true;
    }

    //left bottom sensor segment
    p1 = ccp(player_pos.x+12, player_pos.y+player_h-20);
    p2 = ccp(player_pos.x+12, player_pos.y+player_h+4);

    if( SegmentsTest(p1, p2, o1, o2) ) {
        return true;
    }

    return false;
}

bool GameWorld::SegmentsTest(CCPoint b, CCPoint t, CCPoint l, CCPoint r)
{
    if(l.y > b.y && l.y < t.y) {
        if(l.x < b.x && r.x > b.x){
            return true;
        }
    }

    return false;
}

void GameWorld::PhysicsStep(float dt)
{
    CCPoint player_pos;
    float player_w, player_h;
    player.GetAABB(player_pos, player_w, player_h);
    CCPoint player_rb(player_pos.x + player_w - 1,
                      player_pos.y);
    CCPoint player_lb(player_pos.x, player_pos.y);

    //obstacles test
    for(int i=0; i < obstacles.size(); ++i) {
        CCPoint pos;
        float w, h;
        GObject *obj = obstacles[i];
        obj->GetAABB(pos, w, h);

        if( SideTest(obj) || BottomTest(obj) || TopSideTest(obj) || TopTest(obj)){
            CCLog("bump into obstacle");
            GameOver();
        }
    }

    //check walk intersect with player
    unsigned int n = bottomObjects.size();
    GObject *on_obj=0;
    CCPoint pos;
    float w, h;
    float up_w, up_h;
    for(int i=0; i<n; ++i) {
        GObject *obj = bottomObjects[i];
        obj->GetAABB(pos, w, h);

        //side test
        if( SideTest(obj) ) {
            CCLog("bump into wall");
            GameOver();
        }

        //bottom test
        if( BottomTest(obj) ) {
            on_obj = obj;
            break;
        }

    }

    //check for upper objects
    GObject *on_upObj=0;
    CCPoint upPos;
    n = upperObjects.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = upperObjects[i];
        obj->GetAABB(upPos, up_w, up_h);

        //side test
        if( TopSideTest(obj) ) {
            CCLog("bump into wall");
            GameOver();
        }

        //top test
        if( TopTest(obj) ) {
            on_upObj = obj;
            break;
        }
    }

    if(on_obj) {
        if(player.state != GPlayer::RUN) {
            //CCLog("intersect at a bottom solid tile. set player run");
            player.Run();
            player.SetPlayerPosition(player_pos.x, pos.y+h);
        }
    } else if( on_upObj ) {
        if(player.state != GPlayer::RUN) {
            //CCLog("intersect at a upper solid tile. set player run");
            player.Run();
            player.SetPlayerPosition(player_pos.x, upPos.y-player_h);
        }

    } else{
        //player is on the air
        //CCLog("player is on the air. enable gravity");
        player.EnableGravity();
    }
}

void GameWorld::step(float dt)
{
    //dump_bottomObjects();

    unsigned int n = bottomObjects.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = bottomObjects[i];
        obj->Step(dt);
    }

    n = upperObjects.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = upperObjects[i];
        obj->Step(dt);
    }

    n = obstacles.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = obstacles[i];
        obj->Step(dt);
    }

    RenewMap();
    player.Step(dt);
    PhysicsStep(dt);
    if( player.GetPlayerPosition().y < 10 || player.GetPlayerPosition().y > designSize.height-10) {
        GameOver();
    }

    //distance
    distance += speed;
}

void GameWorld::GameOver()
{
    char s[200];
    snprintf(s, 200, "Score: %d", (int)distance);
    GameOverScene *gameOverScene = GameOverScene::create();
    //gameOverScene->getLayer()->getLabel()->setString("Game Over");
    gameOverScene->getLayer()->getLabel()->setString(s);
	CCDirector::sharedDirector()->replaceScene( gameOverScene );
}


