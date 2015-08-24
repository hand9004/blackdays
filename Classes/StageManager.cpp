#include "StageManager.h"
#include "LuaCommunicator.h"
#include "UI/UIController.h"
#include "Objects/Map.h"
#include "Objects/ObjectController.h"
#include "Scene/SceneManager.h"
#include "Utility/Utility.h"

int l_get_is_object_alive(lua_State* L)
{
	const char* dest_object_name = lua_tostring(L, 1);

	bool isObjectDead = ObjectController::Instance()->getIsObjectDead(dest_object_name);

	lua_pushboolean(L, isObjectDead);

	return 1;
}

int l_get_enemy_cnt(lua_State* L)
{
	unsigned int enemy_cnt = ObjectController::Instance()->getEnemyObjectCnt();

	lua_pushinteger(L, enemy_cnt);

	return 1;
}

int l_get_ally_cnt(lua_State* L)
{
	unsigned int ally_cnt = ObjectController::Instance()->getAllyObjectCnt();

	lua_pushinteger(L, ally_cnt);

	return 1;
}

int l_get_collided_trigger(lua_State* L)
{
	const char* ret_str = nullptr;

	const char* target_obj_name = lua_tostring(L, 1);

	ret_str = ObjectController::Instance()->getCollidedTrigger(target_obj_name);

	lua_pushstring(L, ret_str);

	return 1;
}

int l_send_stage_flag(lua_State* L)
{
	bool isStageClear = false, isVictorious = false;

	INTEGER_TO_BOOLEAN(isStageClear, lua_toboolean(L, 1));
	INTEGER_TO_BOOLEAN(isVictorious, lua_toboolean(L, 2));

	StageManager::Instance()->setStageClear(isStageClear);
	StageManager::Instance()->setVictorious(isVictorious);

	return 0;
}

int l_send_stage_AllClear(lua_State* L)
{
	bool isStageAllClear = false;

	INTEGER_TO_BOOLEAN(isStageAllClear, lua_toboolean(L, 1));

	StageManager::Instance()->setStageAllClear(isStageAllClear);

	return 0;
}

int l_send_stage_index(lua_State* L)
{
	unsigned int st_index = 0;

	st_index = lua_tointeger(L, 1);

	StageManager::Instance()->setStageIndex(st_index);

	return 0;
}

int l_recv_stage_index(lua_State* L)
{
	unsigned ret_stage_index = StageManager::Instance()->getStageIndex();
	
	lua_pushinteger(L, ret_stage_index);

	return 1;
}

int l_set_object_stat(lua_State* L)
{


	return 0;
}

StageManager::StageManager(void) : isStageStart(false), isStageClear(false), isVictorious(false), isStageAllClear(false), isStageUpdateLock(false)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::register_StageFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("get_is_object_alive", l_get_is_object_alive);
	LuaCommunicator::Instance()->Register_CFunction("get_enemy_cnt", l_get_enemy_cnt);
	LuaCommunicator::Instance()->Register_CFunction("get_ally_cnt", l_get_ally_cnt);
	LuaCommunicator::Instance()->Register_CFunction("get_collided_trigger", l_get_collided_trigger);
	LuaCommunicator::Instance()->Register_CFunction("send_stage_flag", l_send_stage_flag);
	LuaCommunicator::Instance()->Register_CFunction("send_stage_AllClear", l_send_stage_AllClear);
	LuaCommunicator::Instance()->Register_CFunction("send_stage_index", l_send_stage_index);
	LuaCommunicator::Instance()->Register_CFunction("recv_stage_index", l_recv_stage_index);
}

void StageManager::init_Manager()
{
}

void StageManager::destroy_Manager()
{
}

void StageManager::init_Stage()
{	
	LuaCommunicator::Instance()->Call_LuaFunction("game_Init", ">");
	LuaCommunicator::Instance()->Call_LuaFunction("sound_Init", ">");

	if(!isStageAllClear)
	{
		ObjectController::Instance()->update_Object();
		ObjectController::Instance()->update_BindUI();
	}
	isStageUpdateLock = false;
}
void StageManager::update_Stage()
{
	UIComponent* finded_ui = UIController::Instance()->find_UIComponent("stage_message");
	if(finded_ui != nullptr)
	{
		bool isSceneChange = SceneManager::Instance()->isSceneChanging();

		if(!isSceneChange)
		{
			LuaCommunicator::Instance()->Call_LuaFunction("game_Update", ">");

			if(!isStageAllClear)
			{
				if(!isStageStart)
				{
					finded_ui->recv_message_main((void*)false);

					if(!finded_ui->get_UIActive())
						isStageStart = true;
					else
						isStageUpdateLock = true;
				}
				else
				{
					if(!isStageClear)
					{
						ObjectController::Instance()->update_BindUI();
						ObjectController::Instance()->update_Object();
						ObjectController::Instance()->update_Dead_Object();

						finded_ui->recv_message_main((void*)false);
						isStageUpdateLock = false;
					}
					else
					{
						if(isVictorious)
						{
							if(!finded_ui->get_UIActive())
							{
								isStageStart = false;

								destroy_Stage();
								init_Stage();
								return ;
							}
							else
							{
								finded_ui->recv_message_main((void*)true);
								isStageUpdateLock = true;
							}
						}
						else
						{
							if(!finded_ui->get_UIActive())
								SceneManager::Instance()->set_SceneReplaceToken("Main");
							else
							{
								finded_ui->recv_message_main((void*)true);
								isStageUpdateLock = true;
							}
						}
					}
				}
			}
			else
			{
				if(!finded_ui->get_UIActive())
					SceneManager::Instance()->set_SceneReplaceToken("Main");
				else
				{
					finded_ui->recv_message_main((void*)true);
					isStageUpdateLock = true;
				}
			}
		}
	}
}
void StageManager::destroy_Stage()
{
	Map::Instance()->Destroy();
	ObjectController::Instance()->clear_Object();

	if(isStageAllClear)
	{
		UIController::Instance()->clear_UILayer();
		UIController::Instance()->clear_UIComponent();
	}
}