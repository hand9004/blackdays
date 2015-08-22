#pragma once
#include <cocos2d.h>
class TouchLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(TouchLayer);
private:
	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

	virtual void keyBackClicked();
	virtual void keyMenuClicked();
private:
	void check_touch_event(float dt);
private:
	cocos2d::CCPoint scroll_delta;
};

