#include "GameScene.h"
#include "TouchLayer.h"
#include "../Objects/Map.h"
#include "../StageManager.h"
#include "../SoundManager.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"
#include "../UI/UIController.h"
#include "../Scene/SceneManager.h"
#include "../System/SystemInfo.h"
#include "../Objects/ObjectController.h"
#include "../Graphics/GraphicsController.h"
#include "../Objects/ParticleController.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }

    return true;
}
void GameScene::onEnter()
{
	CCScene::onEnter();

	TouchLayer* touch_layer = TouchLayer::create();

	addChild(touch_layer);

	ObjectController::Instance()->init();
	UIController::Instance()->init();

	init_SceneInfo();
}
void GameScene::onExit()
{
	CCScene::onExit();
}
void GameScene::schedule_updater(float dt)
{
	SceneManager::Instance()->updateScene();
	StageManager::Instance()->update_Stage();
}
void GameScene::init_SceneInfo()
{
	const char* lua_pt = LuaCommunicator::Instance()->getLuaFileChunk("Script/Game.lua");
	LuaCommunicator::Instance()->Lua_FileOpenByBuffer(lua_pt);
	LuaCommunicator::Instance()->Lua_Register_Functions();

	SystemInfo::Instance()->register_SystemFunction();

	Map::Instance()->register_MapFunction();
	ObjectController::Instance()->register_ObjectFunction();
	StageManager::Instance()->register_StageFunction();

	SceneManager::Instance()->register_SceneFunction();
	UIController::Instance()->register_UIFunctions();
	SoundManager::Instance()->register_SoundFunction();

	LuaCommunicator::Instance()->Call_LuaFunction("init_on_platform", ">");

	LuaCommunicator::Instance()->Call_LuaFunction("ui_Init", ">");
	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");

	StageManager::Instance()->init_Stage();

	schedule(schedule_selector(GameScene::schedule_updater));

	SceneManager::Instance()->set_SceneReplaceToken("");
}

void GameScene::destroy_SceneInfo()
{
	StageManager::Instance()->setStageAllClear(false);
	StageManager::Instance()->setStageClear(false);
	StageManager::Instance()->setVictorious(false);
	unschedule(schedule_selector(GameScene::schedule_updater));
	LuaCommunicator::Instance()->Lua_FileClose();
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}