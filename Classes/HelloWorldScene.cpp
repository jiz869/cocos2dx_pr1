#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

HelloWorld::~HelloWorld()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld()
{
}

CCScene* HelloWorld::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

static int getRandom(int low, int high)
{
	if ( low - high < 0x10000L )
	      return low + ( ( random() >> 8 ) % ( high + 1 - low ) );

	return low + ( random() % ( high + 1 - low ) );
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
			menu_selector(HelloWorld::menuCloseCallback));
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

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		/////////////////////////////
		// 2. add your codes below...

		CCSprite *tree1 = CCSprite::create("tree.png");
		tree1->setPosition( ccp(20,20) );
		tree1->setAnchorPoint( ccp(0.5f, 0) );
		tree1->setScale(1.5f);
		this->addChild( tree1, 2, TAG_TREE_SPRITE_1);

		CCSprite *cat = CCSprite::create("cheshire_cat.png");
		cat->setPosition( ccp(250, 180) );
		cat->setScale( 0.4f );
		this->addChild( cat, 3, TAG_CAT_SPRITE);

		//CCTexture2D
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("tree.png");
		CCSprite *tree2 = CCSprite::createWithTexture( texture );
		tree2->setPosition( ccp(300, 20) );
		tree2->setAnchorPoint( ccp(0.5f, 0) );
		tree2->setScale(2.0f);
		this->addChild( tree2, 2, TAG_TREE_SPRITE_2 );

		//CCSpriteFrameCache
		CCSpriteFrame *frame = CCSpriteFrame::createWithTexture( texture, tree2->getTextureRect() );
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame( frame, "tree.png");
		CCSprite *tree3 = CCSprite::createWithSpriteFrameName("tree.png");
		tree3->setPosition( ccp(400, 20) );
		tree3->setAnchorPoint( ccp(0.5f, 0) );
		tree3->setScale(1.5f);
		this->addChild(tree3, 2, TAG_TREE_SPRITE_3);

		// Load a set of sprite frames from PLIST file
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("alice_scene_sheet.plist");
		CCSprite *alice = CCSprite::createWithSpriteFrameName( "alice.png" );
		alice->getTexture()->generateMipmap();
		ccTexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		alice->getTexture()->setTexParameters( &texParams );

		alice->setPosition( ccp(120,20) );
		alice->setScale(0.4f);
		alice->setAnchorPoint( ccp(0.5f, 0) );
		this->addChild( alice, 2, TAG_ALICE_SPRITE);

		CCSequence *alice_scale = CCSequence::create( CCScaleTo::create(4.0f, 0.7f), CCScaleTo::create(4.0f, 0.1f), NULL );
		CCRepeatForever *repeat_alice_scale = CCRepeatForever::create( alice_scale );
		alice->runAction( repeat_alice_scale );

		CCSpriteBatchNode *cloudBatch = CCSpriteBatchNode::create("cloud_01.png", 10);
		this->addChild( cloudBatch, 1, TAG_CLOUD_BATCH );
		for(int x=0; x<10; x++) {
			CCSprite *s= CCSprite::createWithTexture( cloudBatch->getTexture(), CCRectMake(0,0,64,64) );
			s->setOpacity(100);
			cloudBatch->addChild(s);
			s->setPosition( ccp( getRandom(1, 0x7ffffff)%640-50, getRandom(1, 0x7ffffff)%150+200) );
		}

		//draw colored rectangles using a 1px x 1px white texture
		CCSprite *sky = CCSprite::create("blank.png");
		sky->setPosition( ccp(320, 240) );
		sky->setTextureRect( CCRectMake(0,0,640,260) );
		sky->setColor( ccc3(150, 200, 200) );

		this->setTouchEnabled(true);
		this->addChild( sky, 0);

		// use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		//this->schedule( schedule_selector(HelloWorld::updateGame) );

		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
	} while (0);

	return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#endif
}

// cpp with cocos2d-x
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

}

void HelloWorld::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
