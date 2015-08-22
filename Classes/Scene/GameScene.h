#pragma once
#include <cocos2d.h>
class GameScene : public cocos2d::CCScene
{
private:
	GameScene(void){}
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void init_SceneInfo();

	void destroy_SceneInfo();

	void schedule_updater(float dt);

    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};