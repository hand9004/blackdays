#include "StartScene.h"
#include "TouchLayer.h"
#include "../SoundManager.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"
#include "../UI/UIController.h"
#include "../Scene/SceneManager.h"
#include "../System/SystemInfo.h"
#include "../Graphics/GraphicsController.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(visibleSize.width, visibleSize.height, kResolutionNoBorder);

    return true;
}
void StartScene::onEnter()
{
	CCScene::onEnter();

	TouchLayer* touch_layer = TouchLayer::create();
	addChild(touch_layer);

	UIController::Instance()->init();

	init_SceneInfo();
}
void StartScene::onExit()
{
	CCScene::onExit();
}
void StartScene::scene_schedule_updater(float dt)
{
	SceneManager::Instance()->updateScene();
}
void StartScene::init_SceneInfo()
{
	// 2. operate Lua Script For MainMenu
	const char* lua_pt = LuaCommunicator::Instance()->getLuaFileChunk("Script/Start.lua");
	LuaCommunicator::Instance()->Lua_FileOpenByBuffer(lua_pt);
	LuaCommunicator::Instance()->Lua_Register_Functions();

	SystemInfo::Instance()->register_SystemFunction();
	SceneManager::Instance()->register_SceneFunction();
	UIController::Instance()->register_UIFunctions();
	SoundManager::Instance()->register_SoundFunction();
	
	LuaCommunicator::Instance()->Call_LuaFunction("ui_Init", ">");
	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");

	LuaCommunicator::Instance()->Call_LuaFunction("sound_Init", ">");

	schedule(schedule_selector(StartScene::scene_schedule_updater));

	SceneManager::Instance()->set_SceneReplaceToken("");
}

void StartScene::destroy_SceneInfo()
{
	unschedule(schedule_selector(StartScene::scene_schedule_updater));
	LuaCommunicator::Instance()->Lua_FileClose();
}

void StartScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}