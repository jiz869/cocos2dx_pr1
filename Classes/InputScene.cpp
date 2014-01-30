#include "InputScene.h"

using namespace cocos2d;

InputScene::~InputScene()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

InputScene::InputScene() : _touchSprite(0)
{
}

CCScene* InputScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		InputScene *layer = InputScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}



// on "init" you need to initialize your instance
bool InputScene::init()
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
			menu_selector(InputScene::menuCloseCallback));
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

		// Add the menu to InputScene layer as a child layer.
		this->addChild(pMenu, 0.1);

		///////////////////////////////////////////////
		// 2. add your codes below...
        ///////////////////////////////////////////////
        _touchSprite = CCSprite::create("blank.png");
        _touchSprite->setPosition(ccp(240,160));
        _touchSprite->setTextureRect(CCRectMake(0,0,100,100));
        _touchSprite->setColor( ccc3(0, 255, 0) );
        this->addChild(_touchSprite);

		this->setTouchEnabled(true);
        bRet = true;

	} while (0);

	return bRet;
}

void InputScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#endif
}

// cpp with cocos2d-x
void InputScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

}

bool PointInSprite(CCPoint &p, const CCSprite &sprite)
{
    return false;
}

