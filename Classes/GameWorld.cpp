#include "GameWorld.h"
#include "AnimationScene.h"

using namespace cocos2d;
static bool PointInSprite(CCPoint &p, CCSprite &sprite);

GameWorld::~GameWorld()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
    this->sonic->release();
    this->run->release();
    this->sonic_texture->release();
}

GameWorld::GameWorld() : sonic(0), run(0)
{

}

void GameWorld::sonicRun()
{
    this->sonic->stopAllActions();
    CCAnimate *aa = CCAnimate::create(this->run);
    CCRepeatForever *rep = CCRepeatForever::create(aa);
    this->sonic->runAction(rep);
}

void GameWorld::sonicJumpDownDone()
{
    sonicRun();
}

void GameWorld::sonicJumpDown()
{
    int w=50, h=34;
    float jmpduration = 0.35;
    //this->sonic->setFlipY(true);
    this->sonic->setTextureRect( CCRectMake(6*w, 1*h+1, w, h) );
    CCFiniteTimeAction *movDown = CCMoveBy::create( jmpduration, ccp(0,-100) );
    CCEaseIn *emovDown = CCEaseIn::create( (CCActionInterval*)movDown, 2 );
    CCFiniteTimeAction *movDownDone = CCCallFuncN::create( this, callfuncN_selector(GameWorld::sonicJumpDownDone) );
    this->sonic->runAction( CCSequence::create(emovDown, movDownDone, NULL) );
}

void GameWorld::sonicJumpUpDone()
{
    sonicJumpDown();
}

void GameWorld::sonicJumpUp()
{
    int w=50, h=34;
    float jmpduration = 0.3;
    this->sonic->setTextureRect( CCRectMake(5*w, 1*h+1, w, h) );
    CCFiniteTimeAction *movUp = CCMoveBy::create( jmpduration, ccp(0,100) );
    CCEaseOut *emovUp = CCEaseOut::create( (CCActionInterval*)movUp, 2 );

    CCFiniteTimeAction *movUpDone = CCCallFuncN::create( this, callfuncN_selector(GameWorld::sonicJumpUpDone) );

    this->sonic->runAction( CCSequence::create(emovUp, movUpDone, NULL) );
}

void GameWorld::LoadCharacter()
{
    int w=50, h=34;
    this->sonic_texture = CCTextureCache::sharedTextureCache()->addImage("sonic.png");
    this->sonic_texture->retain();

    this->sonic = CCSprite::createWithTexture(this->sonic_texture, CCRectMake(0,0, w, h));
    this->sonic->retain();
    this->sonic->setPosition(ccp(100,100));
    this->addChild(this->sonic);

    //load animation
    this->run = CCAnimation::create();
    this->run->retain();
    this->run->setDelayPerUnit(1.0/8.0);

    for(int i=0; i<8; ++i) {
        CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->sonic_texture, CCRectMake(i*w, 0, w, h));
        this->run->addSpriteFrame(frame);
    }

    CCAnimate *aa = CCAnimate::create(this->run);
    CCRepeatForever *rep = CCRepeatForever::create(aa);
    this->sonic->runAction(rep);
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
        //this->schedule( schedule_selector(GameWorld::step) );
        this->LoadCharacter();


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
    this->sonic->stopAllActions();
    sonicJumpUp();
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

