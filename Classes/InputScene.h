#ifndef __INPUT_SCENE_H__
#define __INPUT_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

//////////////////////////////////////////////////////////////////
class InputScene : public cocos2d::CCLayerColor
{
public:
	InputScene();
	~InputScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(InputScene);

    //implement base virtual functions
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

protected:
};

#endif  // __INPUT_SCENE_H__

