#ifndef __ANIMATION_SCENE_H__
#define __ANIMATION_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

enum {
	TAG_CASTLE = 0,
	TAG_LIGHTNING_BOLT = 1,
	TAG_BATS = 2
};

//Bat animation types
enum {
	BAT_FLYING_UP = 0,
	BAT_GLIDING_DOWN = 1
};

////////// define of SimpleAnimObject //////////////////
class SimpleAnimObject : public CCSprite {
    public:
        virtual ~SimpleAnimObject() {
        };

        // data
        int animationType;
        CCPoint velocity;

        virtual void update( float time ) {
            this->m_obPosition = ccp( this->m_obPosition.x + velocity.x, this->m_obPosition.y + velocity.y );
        }

        CCRect rect() {
            float scaleMod = 0.5f;
            float w = this->getContentSize().width * this->getScale() * scaleMod;
            float h = this->getContentSize().height * this->getScale() * scaleMod;
            CCPoint point = CCPointMake( this->m_obPosition.x - w/2 , this->m_obPosition.y - h/2 );
            return CCRectMake( point.x, point.y, w, h );
        };

        CCSprite *sprite;

        CREATE_FUNC(SimpleAnimObject);
};

//////////////////////////////////////////////////////////////////
class AnimationScene : public cocos2d::CCLayerColor
{
public:
	AnimationScene();
	~AnimationScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(AnimationScene);

	void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    float makeBatFlyUp( SimpleAnimObject *bat );
    void makeBatGlideDown( SimpleAnimObject *bat );
    void step( float delta );

protected:
    SimpleAnimObject *bats[20];
    CCAnimation *batFlyUp;
    CCAnimation *batGlideDown;
    CCSprite *lightningBolt;
    CCSprite *lightningGlow;
    int lightningRemoveCount;
};

#endif  // __ANIMATION_SCENE_H__

