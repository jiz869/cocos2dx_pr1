#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

enum {
	TAG_ALICE_SPRITE = 0,
	TAG_CAT_SPRITE = 1,
	TAG_TREE_SPRITE_1 = 2,
	TAG_TREE_SPRITE_2 = 3,
	TAG_TREE_SPRITE_3 = 4,
	TAG_CLOUD_BATCH = 5,
	TAG_GRASS_BATCH_1 = 6,
	TAG_GRASS_BATCH_2 = 7
};

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);

	void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);


protected:

};

#endif  // __HELLOWORLD_SCENE_H__
