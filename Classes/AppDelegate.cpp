#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "Scene/SceneManager.h"
#include "SimpleAudioEngine.h"
#include "UI/UIController.h"


USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
#ifdef DEBUG_MODE
    pDirector->setDisplayStats(true);
#else
	pDirector->setDisplayStats(false);
#endif
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(800, 480, ResolutionPolicy::kResolutionNoBorder);

	SceneManager::Instance()->createScene(MAIN);

    // run
	pDirector->runWithScene(SceneManager::Instance()->getCurrentScene());
    
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

	cocos2d::CCDirector::sharedDirector()->pause();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

	cocos2d::CCDirector::sharedDirector()->resume();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
