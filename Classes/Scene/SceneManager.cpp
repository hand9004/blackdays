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

const int vibration_range = 5;

int l_replace_scene(lua_State* L)
{
	SceneManager* scene_mgr = SceneManager::Instance();

	scene_mgr->set_SceneReplaceToken(lua_tostring(L, 1));

	return 0;
}

int l_set_camera_is_fixed(lua_State* L)
{
	bool isCameraFixed = false;
	INTEGER_TO_BOOLEAN(isCameraFixed, lua_toboolean(L, 1));

	lua_pop(L, 1);

	SceneManager::Instance()->setIsCameraFixed(isCameraFixed);

	return 0;
}

int l_control_scheduler(lua_State* L)
{
	bool isPaused = false;
	const char* scene_type = lua_tostring(L, 1);
	INTEGER_TO_BOOLEAN(isPaused, lua_toboolean(L, 2));

	lua_pop(L, 2);

	SceneManager::Instance()->control_scheduler(scene_type, isPaused);

	return 0;
}

int l_vibrate_scene(lua_State* L)
{
	bool isVibrate = false;
	INTEGER_TO_BOOLEAN(isVibrate, lua_toboolean(L, 1));
	long vibrate_duration_ms = lua_tointeger(L, 2);

	lua_pop(L, 2);

	SceneManager::Instance()->setSceneVibration(isVibrate, vibrate_duration_ms);

	return 0;
}

SceneManager::SceneManager(void) : replace_scene_token(""), current_Scene(nullptr), isCameraFixed(false), isGamePaused(false), isSceneVibration(false), 
								vibrate_start_time(0), vibrate_end_time(0)
{
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::register_SceneFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("replace_scene", l_replace_scene);
	LuaCommunicator::Instance()->Register_CFunction("set_camera_is_fixed", l_set_camera_is_fixed);
	LuaCommunicator::Instance()->Register_CFunction("control_scheduler", l_control_scheduler);
	LuaCommunicator::Instance()->Register_CFunction("vibrate_scene", l_vibrate_scene);
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

	update_scene_vibration();
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

	BD_CrtDumpMemoryLeaks();
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

void SceneManager::control_scheduler(const char* scene_type, bool isPaused)
{
	if (!strcmp(scene_type, "Main"))
	{
		StartScene* start_inst = dynamic_cast<StartScene*>(current_Scene);
		
		CCAssert(start_inst != nullptr, "StartScene is nullptr.");
		if (isPaused)
			start_inst->pause_scheduler();
		else
			start_inst->resume_scheduler();
	}
	else if (!strcmp(scene_type, "Select"))
	{
		StageSelectScene* select_inst = dynamic_cast<StageSelectScene*>(current_Scene);

		CCAssert(select_inst != nullptr, "StageSelectScene is nullptr.");
		if (isPaused)
			select_inst->pause_scheduler();
		else
			select_inst->resume_scheduler();
	}
	else if (!strcmp(scene_type, "Game"))
	{
		GameScene* game_inst = dynamic_cast<GameScene*>(current_Scene);

		CCAssert(game_inst != nullptr, "GameScene is nullptr.");
		if (isPaused)
			game_inst->pause_scheduler();
		else
			game_inst->resume_scheduler();
	}
}
void SceneManager::update_scene_vibration()
{
	if (isSceneVibration)
	{
		if (current_Scene != nullptr)
		{
			long delta_time = vibrate_end_time - vibrate_start_time;

			current_Scene->setPosition(CCPoint(0.f, 0.f));
			if (delta_time > vibrate_duration_ms)
			{
				vibrate_start_time = get_ms_onSystem();
				vibrate_end_time = get_ms_onSystem();

				isSceneVibration = false;
			}
			else
			{
				srand(get_ms_onSystem());

				bool isMinusX = false, isMinusY = false;
				INTEGER_TO_BOOLEAN(isMinusX, rand() % 2);
				INTEGER_TO_BOOLEAN(isMinusY, rand() % 2);

				float rand_X = rand() % vibration_range, rand_Y = rand() % vibration_range;

				rand_X = (isMinusX) ? -rand_X : rand_X;
				rand_Y = (isMinusY) ? -rand_Y : rand_Y;

				CCPoint scene_position = current_Scene->getPosition();

				scene_position.x += rand_X;
				scene_position.y += rand_Y;

				current_Scene->setPosition(scene_position);

				vibrate_end_time = get_ms_onSystem();
				isSceneVibration = true;
			}
		}
	}
	else
	{
		current_Scene->setPosition(CCPoint(0.f, 0.f));

		vibrate_start_time = get_ms_onSystem();
		vibrate_end_time = get_ms_onSystem();
	}
}