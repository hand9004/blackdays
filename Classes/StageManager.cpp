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

	lua_pop(L, 1);

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

	lua_pop(L, 1);

	ret_str = ObjectController::Instance()->getCollidedTrigger(target_obj_name);

	lua_pushstring(L, ret_str);

	return 1;
}

int l_send_stage_flag(lua_State* L)
{
	bool isStageClear = false, isVictorious = false;

	INTEGER_TO_BOOLEAN(isStageClear, lua_toboolean(L, 1));
	INTEGER_TO_BOOLEAN(isVictorious, lua_toboolean(L, 2));

	lua_pop(L, 2);

	StageManager::Instance()->setStageClear(isStageClear);
	StageManager::Instance()->setVictorious(isVictorious);

	return 0;
}

int l_send_stage_AllClear(lua_State* L)
{
	bool isStageAllClear = false;

	INTEGER_TO_BOOLEAN(isStageAllClear, lua_toboolean(L, 1));

	lua_pop(L, 1);

	StageManager::Instance()->setStageAllClear(isStageAllClear);

	return 0;
}

int l_send_stage_index(lua_State* L)
{
	unsigned int st_index = 0;

	st_index = lua_tointeger(L, 1);

	lua_pop(L, 1);

	StageManager::Instance()->setStageIndex(st_index);

	return 0;
}

int l_recv_stage_index(lua_State* L)
{
	unsigned ret_stage_index = StageManager::Instance()->getStageIndex();
	
	lua_pushinteger(L, ret_stage_index);

	return 1;
}

int l_set_talk_event(lua_State* L)
{
	const char* conversation_table_name = lua_tostring(L, 1);

	StageManager::Instance()->add_talk_event(conversation_table_name);

	lua_pop(L, 1);

	return 0;
}

int l_active_talk_event(lua_State* L)
{
	bool isTalkActive = false;
	const char* talk_event_name = lua_tostring(L, 1);
	INTEGER_TO_BOOLEAN(isTalkActive, lua_toboolean(L, 2));

	StageManager::Instance()->active_talk_event(talk_event_name, isTalkActive);

	return 0;
}

int l_set_conversation_change(lua_State* L)
{
	bool isChangeConv = false;
	INTEGER_TO_BOOLEAN(isChangeConv, lua_toboolean(L, 1));

	StageManager::Instance()->setConversationChange(isChangeConv);

	return 0;
}

int l_get_current_talk(lua_State* L)
{
	const char* curr_talk_name = StageManager::Instance()->getCurrentTalkName();

	lua_pushstring(L, curr_talk_name);

	return 1;
}

int l_get_current_conversation(lua_State* L)
{
	unsigned int curr_conversation = StageManager::Instance()->getCurrentConversationIndex();

	lua_pushinteger(L, curr_conversation);

	return 1;
}

int l_get_is_stage_start(lua_State* L)
{
	bool isStageStart = StageManager::Instance()->getStageStart();

	lua_pushboolean(L, isStageStart);

	return 1;
}

int l_get_is_talk_start(lua_State* L)
{
	bool isTalkStart = StageManager::Instance()->getTalkEvent();

	lua_pushboolean(L, isTalkStart);

	return 1;
}

StageManager::StageManager(void) : isStageStart(false), isStageClear(false), isVictorious(false), isStageAllClear(false), isStageUpdateLock(false),
isTalkEvent(false), onConversationChange(false), isGameRestrict(false), isUIRestrict(false),
left_img(nullptr), right_img(nullptr), message_background(nullptr), message_font(nullptr), current_conversation(0)
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
	LuaCommunicator::Instance()->Register_CFunction("set_talk_event", l_set_talk_event);
	LuaCommunicator::Instance()->Register_CFunction("active_talk_event", l_active_talk_event);
	LuaCommunicator::Instance()->Register_CFunction("set_conversation_change", l_set_conversation_change);
	LuaCommunicator::Instance()->Register_CFunction("get_current_talk", l_get_current_talk);
	LuaCommunicator::Instance()->Register_CFunction("get_current_conversation", l_get_current_conversation);
	LuaCommunicator::Instance()->Register_CFunction("get_is_stage_start", l_get_is_stage_start);
	LuaCommunicator::Instance()->Register_CFunction("get_is_talk_start", l_get_is_talk_start);
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

	init_talk_event();
	isStageUpdateLock = false;
}
void StageManager::update_Stage()
{
	UIComponent* finded_ui = UIController::Instance()->find_UIComponent("stage_message");
	if(finded_ui != nullptr)
	{
		bool isSceneChange = SceneManager::Instance()->isSceneChanging();

		pop_message_msg_recv* stage_message_msg = new pop_message_msg_recv;
		stage_message_msg->onChange = false;

		if(!isSceneChange)
		{
			LuaCommunicator::Instance()->Call_LuaFunction("game_Update", ">");

			if(!isStageAllClear)
			{
				if(!isStageStart)
				{
					stage_message_msg->onPopup = false;
					finded_ui->recv_message_main(stage_message_msg);

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
						update_talk_event();

						stage_message_msg->onPopup = false;
						finded_ui->recv_message_main(stage_message_msg);
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
								stage_message_msg->onPopup = true;
								finded_ui->recv_message_main(stage_message_msg);
								isStageUpdateLock = true;
							}
						}
						else
						{
							if (!finded_ui->get_UIActive())
							{
								destroy_Stage();
								isStageStart = false;
								SceneManager::Instance()->set_SceneReplaceToken("Main");
							}
							else
							{
								stage_message_msg->onPopup = true;
								finded_ui->recv_message_main(stage_message_msg);
								isStageUpdateLock = true;
							}
						}
					}
				}
			}
			else
			{
				if (!finded_ui->get_UIActive())
				{
					destroy_Stage();
					SceneManager::Instance()->set_SceneReplaceToken("Main");
				}
				else
				{
					stage_message_msg->onPopup = true;
					finded_ui->recv_message_main(stage_message_msg);
					isStageUpdateLock = true;
				}
			}
		}
		else
		{
			UIController::Instance()->active_Layer("conversation_layer", false);
			isStageStart = false;
		}

		SAFE_DELETE(stage_message_msg);
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

	isStageStart = isStageClear = isVictorious = isStageAllClear = isStageUpdateLock = false;
}
void StageManager::add_talk_event(const char* table_name)
{
	lua_State* L = LuaCommunicator::Instance()->Lua_GetState();

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	talk_info* one_talk_info = new talk_info;

	lua_pushstring(L, "talk_dialog");
	lua_gettable(L, 2);

	one_talk_info->message_background = lua_tostring(L, -1);

	lua_pop(L, 1);

	const unsigned int global_attri_cnt = 1;
	unsigned int talk_attri_cnt = LuaCommunicator::Instance()->getTableSize(2);
	for (unsigned int i = 1; i <= talk_attri_cnt - global_attri_cnt; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, 2);

		s_conversation_info* s_conv_info = new s_conversation_info;

		lua_pushstring(L, "restriction");
		lua_gettable(L, 3);

		lua_pushstring(L, "left");
		lua_gettable(L, 3);

		lua_pushstring(L, "right");
		lua_gettable(L, 3);

		lua_pushstring(L, "message");
		lua_gettable(L, 3);

		lua_pushstring(L, "font_size");
		lua_gettable(L, 3);

		s_conv_info->restriction = lua_tostring(L, -5);
		s_conv_info->left_portrait_img_name = lua_tostring(L, -4);
		s_conv_info->right_portrait_img_name = lua_tostring(L, -3);
		s_conv_info->message = lua_tostring(L, -2);
		s_conv_info->font_size = lua_tointeger(L, -1);

		lua_pop(L, 5);

		lua_pushstring(L, "font_color");
		lua_gettable(L, 3);

		unsigned int color_attri_cnt = LuaCommunicator::Instance()->getTableSize(4);
		for (unsigned k = 1; k <= color_attri_cnt; ++k)
		{
			lua_pushinteger(L, k);
			lua_gettable(L, 4);
		}

		s_conv_info->message_color.r = lua_tonumber(L, -3);
		s_conv_info->message_color.g = lua_tonumber(L, -2);
		s_conv_info->message_color.b = lua_tonumber(L, -1);

		lua_pop(L, color_attri_cnt + 1);
		
		lua_pop(L, 1);

		one_talk_info->conversation_list.push_back(s_conv_info);
	}

	talk_info_list.insert(std::pair<const char*, talk_info*>(table_name, one_talk_info));

	current_talk = talk_info_list.begin();
}
void StageManager::active_talk_event(const char* talk_event_name, bool isActive)
{
	current_talk = talk_info_list.find(talk_event_name);
	isTalkEvent = onConversationChange = isActive;
}
void StageManager::init_talk_event()
{
	message_font = UIController::Instance()->find_UIComponent("stage_talk_message");
	message_background = UIController::Instance()->find_UIComponent("stage_talk_message_background");
	left_img = UIController::Instance()->find_UIComponent("stage_talk_left_img");
	right_img = UIController::Instance()->find_UIComponent("stage_talk_right_img");

	CCAssert(message_font != nullptr, "Can't Find message_font!");
	CCAssert(message_background != nullptr, "Can't Find message_background!");
	CCAssert(left_img != nullptr, "Can't Find left_img!");
	CCAssert(right_img != nullptr, "Can't Find right_img!");
}
void StageManager::update_talk_event()
{
	if (isTalkEvent)
	{
		if (onConversationChange)
		{
			if (!talk_info_list.empty())
			{
				auto begin = talk_info_list.begin();
				auto end = talk_info_list.end();

				if (current_talk != end)
				 {
					if (!current_talk->second->conversation_list.empty())
					{
						unsigned int conv_cnt = current_talk->second->conversation_list.size();
						if (current_conversation < conv_cnt)
						{
							UIController::Instance()->active_Layer("conversation_layer", true);

							s_conversation_info* conv_pt = current_talk->second->conversation_list.at(current_conversation);

							bg_msg_recv left_msg, right_msg;
							pop_message_msg_recv message_msg;

							left_msg.change_image_path = conv_pt->left_portrait_img_name;
							right_msg.change_image_path = conv_pt->right_portrait_img_name;

							strcpy(message_msg.message, conv_pt->message);
							message_msg.change_color = conv_pt->message_color;
							message_msg.font_size = conv_pt->font_size;
							message_msg.onChange = true;
							message_msg.onPopup = true;

							message_font->recv_message_main(&message_msg);
							left_img->recv_message_main(&left_msg);
							right_img->recv_message_main(&right_msg);

							if (!strcmp(conv_pt->restriction, "UI"))
							{
								isGameRestrict = false;
								isUIRestrict = true;
							}
							else if (!strcmp(conv_pt->restriction, "Game"))
							{
								isGameRestrict = true;
								isUIRestrict = false;
							}
							else if (!strcmp(conv_pt->restriction, "No Restrict"))
							{
								isGameRestrict = false;
								isUIRestrict = false;
							}

							onConversationChange = false;
						}
						else
						{
							UIController::Instance()->active_Layer("conversation_layer", false);
							current_conversation = 0;
							isGameRestrict = false;
							isUIRestrict = false;
							onConversationChange = false;
							isTalkEvent = false;
							++current_talk;
						}
					}
				}
				else
				{
					UIController::Instance()->active_Layer("conversation_layer", false);

					for (auto i = begin; i != end; ++i)
						SAFE_DELETE(i->second);

					map_clear(talk_info_list);
					current_talk = talk_info_list.begin();
					isGameRestrict = false;
					isUIRestrict = false;
					onConversationChange = false;
					isTalkEvent = false;
				}
			}
		}
	}
	else
		UIController::Instance()->active_Layer("conversation_layer", false);
}
void StageManager::destroy_talk_event()
{

}