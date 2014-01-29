#include "AnimationScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

static int getRandom(int low, int high)
{
	if ( low - high < 0x10000L )
	      return low + ( ( random() >> 8 ) % ( high + 1 - low ) );

	return low + ( random() % ( high + 1 - low ) );
}

//////////////////////////////////////////////////////////////

float AnimationScene::makeBatFlyUp( SimpleAnimObject *bat )
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    //Randomize animation speed
    float delay = (float)(getRandom(0, 0x7fff)%5+5)/80;
    //CCAnimation *animation = CCAnimation::createWithSpriteFrames(NULL, delay);
    CCAnimation *animation = CCAnimation::create();
    animation->setDelayPerUnit(delay);

    //Randomize animation frame order
    int num = getRandom(0, 0xffff)%4 + 1;
    char framename[50]={0};

    for(int i=1; i<=4; ++i) {

        sprintf( framename, "simple_bat_0%i.png", num );
        CCSpriteFrame *frame = cache->spriteFrameByName( framename );
        animation->addSpriteFrame( frame );
        num++;
        if(num>4) num = 1;
    }

    bat->sprite->stopAllActions();
    CCAnimate *animateAction = CCAnimate::create(animation);
    CCRepeatForever *repAnim = CCRepeatForever::create( animateAction );
    bat->sprite->runAction(repAnim);

    bat->animationType = BAT_FLYING_UP;

    return delay;
}

void AnimationScene::makeBatGlideDown( SimpleAnimObject* bat )
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCAnimation *animation = CCAnimation::create();
    animation->setDelayPerUnit(100.0f);
    animation->addSpriteFrame( cache->spriteFrameByName("simple_bat_01.png") );

    bat->sprite->stopAllActions();
    CCRepeatForever *repAnim = CCRepeatForever::create( CCAnimate::create(animation) );
    bat->sprite->runAction(repAnim);
    bat->animationType = BAT_GLIDING_DOWN;
}

void AnimationScene::step( float delta )
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    SimpleAnimObject *bat = NULL;
    //CCARRAY_FOREACH( bats, objbat) {
    for( int i=0; i<10; ++i) {
    	bat = bats[i];
        if(bat == NULL ) break;
        CCPoint position = bat->sprite->getPosition();
        if(position.x > s.width) {
            bat->velocity = ccp( -bat->velocity.x, bat->velocity.y );
            bat->sprite->setFlipX(false);
        }else if(position.x < 0) {
            bat->velocity = ccp( -bat->velocity.x, bat->velocity.y );
            bat->sprite->setFlipX(true);
        }else if(position.y > s.height) {
            bat->velocity = ccp( bat->velocity.x, -bat->velocity.y );
            makeBatGlideDown(bat);
        }else if(position.y < 0) {
            bat->velocity = ccp( bat->velocity.x, -bat->velocity.y);
            makeBatFlyUp(bat);
        }

        //randomly make them fly back up
        if(getRandom(0, 0x7fff)%100 == 7) {
            if(bat->animationType == BAT_GLIDING_DOWN) {
                makeBatFlyUp(bat);
                bat->velocity = ccp(bat->velocity.x, -bat->velocity.y);
            }else if(bat->animationType == BAT_FLYING_UP) {
                makeBatGlideDown(bat);
                bat->velocity = ccp(bat->velocity.x, -bat->velocity.y);
            }
        }

        //update bat position
        position.x += bat->velocity.x;
        position.y += bat->velocity.y;
        bat->sprite->setPosition(position);
    }

}

///////////////////////////////////////////////////////

AnimationScene::~AnimationScene()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

AnimationScene::AnimationScene()
{
}

CCScene* AnimationScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		AnimationScene *layer = AnimationScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}



// on "init" you need to initialize your instance
bool AnimationScene::init()
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
			menu_selector(AnimationScene::menuCloseCallback));
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

		// Add the menu to AnimationScene layer as a child layer.
		this->addChild(pMenu, 0.1);

		///////////////////////////////////////////////
		// 2. add your codes below...

        // add background castle
        CCSprite *castle = CCSprite::create("dracula_castle.jpg");
        castle->setPosition( ccp(320, 170) );
        castle->setScale(1.4);
        this->addChild( castle, 0, TAG_CASTLE);


        //bat animation
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("simple_bat.plist");

        CCSpriteBatchNode *batch1 = CCSpriteBatchNode::create( "simple_bat.png", 10 );
        this->addChild( batch1, 2, TAG_BATS);

        for(int x=0; x<10; ++x) {
            CCSprite *bat_sprite = (SimpleAnimObject*) CCSprite::create( "simple_bat.png", CCRectMake(0, 0, 48, 48) );
        	SimpleAnimObject *bat = SimpleAnimObject::create();
            bat->sprite = bat_sprite;
            batch1->addChild(bat_sprite);

            bat_sprite->setPosition( ccp(getRandom(0, 0x7fffff)%400+40, getRandom(0, 0x7fffff)%150+150) );

            float flappingSpeed = makeBatFlyUp( bat );
            //float flappingSpeed = 1.0f;

            //Base y velocity on flappingSpeed

            bat->velocity = ccp( (getRandom(0, 0x7fffff)%1000)/500+0.2f, 0.1f/flappingSpeed );
            bats[x] = bat;
            bat->retain();
            bat_sprite->retain();

            if(bat->velocity.x > 0) {
                bat->sprite->setFlipX(true);
            }
        }

        ///////////////////////////////////////////////
		this->setTouchEnabled(true);

        this->schedule( schedule_selector(AnimationScene::step) );

		// use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		//this->schedule( schedule_selector(AnimationScene::updateGame) );

		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
	} while (0);

	return bRet;
}

void AnimationScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#endif
}

// cpp with cocos2d-x
void AnimationScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

}

//void AnimationScene::registerWithTouchDispatcher()
//{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
//}

