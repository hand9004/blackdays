#include "StageSelectScene.h"
#include "TouchLayer.h"
#include "SceneManager.h"
#include "../StageManager.h"
#include "../SoundManager.h"
#include "../System/SystemInfo.h"
#include "../UI/UIController.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool StageSelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }

    return true;
}
void StageSelectScene::onEnter()
{
	CCScene::onEnter();

	TouchLayer* touch_layer = TouchLayer::create();

	addChild(touch_layer);

	UIController::Instance()->init();

	init_SceneInfo();
}
void StageSelectScene::onExit()
{
	CCScene::onExit();
}
void StageSelectScene::schedule_updater(float dt)
{
	SceneManager::Instance()->updateScene();
}
void StageSelectScene::init_SceneInfo()
{
	const char* lua_pt = LuaCommunicator::Instance()->getLuaFileChunk("Script/Select.lua");
	LuaCommunicator::Instance()->Lua_FileOpenByBuffer(lua_pt);
	LuaCommunicator::Instance()->Lua_Register_Functions();

	StageManager::Instance()->register_StageFunction();
	SceneManager::Instance()->register_SceneFunction();
	SystemInfo::Instance()->register_SystemFunction();
	UIController::Instance()->register_UIFunctions();
	SoundManager::Instance()->register_SoundFunction();

	LuaCommunicator::Instance()->Call_LuaFunction("ui_Init", ">");
	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");

	schedule(schedule_selector(StageSelectScene::schedule_updater));

	SceneManager::Instance()->set_SceneReplaceToken("");
}

void StageSelectScene::destroy_SceneInfo()
{
	unschedule(schedule_selector(StageSelectScene::schedule_updater));
	LuaCommunicator::Instance()->Lua_FileClose();
}

void StageSelectScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}