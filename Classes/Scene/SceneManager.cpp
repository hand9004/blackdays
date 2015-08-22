#include "SceneManager.h"
#include "StartScene.h"
#include "StageSelectScene.h"
#include "GameScene.h"

#include "../Objects/Map.h"
#include "../UI/UIController.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"
#include "../StageManager.h"
#include "../Objects/ObjectController.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;

int replace_Scene(lua_State* L)
{
	SceneManager* scene_mgr = SceneManager::Instance();
	scene_mgr->set_SceneReplaceToken(lua_tostring(L, 1));
	return 0;
}

int l_set_Camera_Is_Fixed(lua_State* L)
{
	bool isCameraFixed = false;
	isCameraFixed = lua_tointeger(L, 1);

	lua_pop(L, 1);

	SceneManager::Instance()->setIsCameraReset(false);
	SceneManager::Instance()->setIsCameraFixed(isCameraFixed);

	SceneManager::Instance()->set_in_UI_CameraFixed(isCameraFixed);

	return 0;
}

SceneManager::SceneManager(void) : replace_scene_token(""), current_Scene(nullptr), isCameraFixed(false), isCameraReset(false)
{
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::register_SceneFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("replace_scene", replace_Scene);
	LuaCommunicator::Instance()->Register_CFunction("set_Camera_Is_Fixed", l_set_Camera_Is_Fixed);
}

void SceneManager::createScene(scene_type type)
{
	cocos2d::CCScene* hasRunningScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
	current_scene_type = type;
	switch(type)
	{
	case MAIN:
		current_Scene = StartScene::create();
		break;
	case SELECT:
		current_Scene = StageSelectScene::create();
		break;
	case GAME:
		current_Scene = GameScene::create();
		break;
	}
	if(hasRunningScene != nullptr)
		cocos2d::CCDirector::sharedDirector()->replaceScene(current_Scene);
}

void SceneManager::updateScene()
{
	SceneManager* scene_mgr = SceneManager::Instance();
	const char* ret_tok = scene_mgr->get_SceneReplaceToken();

	if(!strcmp(ret_tok, "Main"))
	{
		scene_mgr->destroyScene();
		scene_mgr->createScene(MAIN);
	}
	else if(!strcmp(ret_tok, "Select"))
	{
		scene_mgr->destroyScene();
		scene_mgr->createScene(SELECT);
	}
	else if(!strcmp(ret_tok, "Game"))
	{
		scene_mgr->destroyScene();
		scene_mgr->createScene(GAME);
	}
}

void SceneManager::destroyScene()
{
	StartScene* start_inst = dynamic_cast<StartScene*>(current_Scene);
	StageSelectScene* select_inst = dynamic_cast<StageSelectScene*>(current_Scene);
	GameScene* game_inst = dynamic_cast<GameScene*>(current_Scene);

	BD_CCLog("start = %d stage = %d game = %d", start_inst, select_inst, game_inst);
	
	Map::Instance()->Destroy();
	ObjectController::Instance()->clear_Object();

	UIController::Instance()->clear_UILayer();
	UIController::Instance()->clear_UIComponent();

	cocos2d::CCFileUtils::sharedFileUtils()->purgeCachedEntries();
	cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	current_Scene->removeAllChildrenWithCleanup(true);

	if(start_inst != nullptr)
		start_inst->destroy_SceneInfo();
	else if(select_inst != nullptr)
		select_inst->destroy_SceneInfo();
	else if(game_inst != nullptr)
		game_inst->destroy_SceneInfo();
}

cocos2d::CCScene* SceneManager::getCurrentScene()
{
	CCAssert(current_Scene != nullptr, "Current Scene Is NULL. Can't Get Current Scene.");
	return current_Scene;
}

void SceneManager::clearScene()
{
	LuaCommunicator::Instance()->Lua_FileClose();
}