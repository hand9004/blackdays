#include "ObjectController.h"
#include "../LuaCommunicator.h"
#include "../Graphics/GraphicsController.h"
#include "../UI/UIController.h"
#include "../UI/BD_Button.h"
#include "../Utility/Utility.h"
#include "ParticleController.h"
#include "../StageManager.h"
#include "Map.h"

USING_NS_CC;

int l_add_Object(lua_State* L)
{
	const char *table_name = nullptr;

	table_name = lua_tostring(L, 1);
	int posX = lua_tointeger(L, 2);
	int posY = lua_tointeger(L, 3);

	lua_pop(L, 3);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	CCPoint curr_map_start_pt = Map::Instance()->getMapStartPoint();

	GameObject* created_obj = ObjectController::Instance()->add_Object(posX + curr_map_start_pt.x, posY);

	lua_pushinteger(L, (int)created_obj);

	return 1;
}

int l_update_Object(lua_State* L)
{
	return 0;
}

int l_remove_Object(lua_State* L)
{
	return 0;
}

int l_command_to_Object(lua_State* L)
{
	unsigned int object_id = lua_tointeger(L, 1);
	const char* command_str = lua_tostring(L, 2);

	GameObject* finded_obj = ObjectController::Instance()->getObject(object_id);

	if (!strcmp(command_str, "Move"))
	{
		int pos_X = lua_tointeger(L, 3);
		int pos_Y = lua_tointeger(L, 4);

		CCPoint dest_pt = CCPoint(pos_X, pos_Y);

		finded_obj->Move(dest_pt);

		lua_pop(L, 4);
	}
	else if (!strcmp(command_str, "Skill"))
	{
		
	}
	else if (!strcmp(command_str, "Patrol"))
	{
		int pos_X = lua_tointeger(L, 3);
		int pos_Y = lua_tointeger(L, 4);
		unsigned int patrol_delay_time = lua_tointeger(L, 5);
		CCPoint dest_pt = CCPoint(pos_X, pos_Y);

		CCPoint scrolled_pt =  Map::Instance()->getMapStartPoint();

		finded_obj->Patrol(dest_pt + scrolled_pt, patrol_delay_time);

		lua_pop(L, 4);
	}

	return 0;
}

int l_set_ObjectInfo(lua_State* L)
{
	unsigned int object_id = lua_tointeger(L, 1);
	const char* set_object_stat = lua_tostring(L, 2);

	GameObject* finded_obj = ObjectController::Instance()->getObject(object_id);
	obj_info* finded_obj_info = &finded_obj->getObjectInfo();
	if (!strcmp(set_object_stat, "max_health"))
	{
		unsigned int max_health_pt = lua_tointeger(L, 3);

		if (finded_obj_info->health_point > max_health_pt)
			finded_obj_info->health_point = max_health_pt;

		finded_obj_info->max_health_point = max_health_pt;
	}
	else if (!strcmp(set_object_stat, "attack_point"))
	{
		unsigned int atk_pt = lua_tointeger(L, 3);

		finded_obj_info->attack_point = atk_pt;
	}
	else if (!strcmp(set_object_stat, "defense_point"))
	{
		unsigned int def_pt = lua_tointeger(L, 3);

		finded_obj_info->defense_point = def_pt;
	}
	else if (!strcmp(set_object_stat, "move_speed"))
	{
		unsigned int move_speed = lua_tointeger(L, 3);

		finded_obj_info->move_speed = move_speed;
	}
	else if (!strcmp(set_object_stat, "patrol_delay_time"))
	{
		unsigned int patrol_delay_time = lua_tointeger(L, 3);

		finded_obj_info->patrol_delay_time = patrol_delay_time;
	}

	lua_pop(L, 3);

	return 0;
}

int l_set_AllObject_Event(lua_State* L)
{
	unsigned int obj_ev = lua_tointeger(L, 1);

	ObjectController::Instance()->setAllObjectEvent(static_cast<obj_event>(obj_ev));

	return 0;
}

int l_get_status_on_object(lua_State* L)
{
	const char* object_name = lua_tostring(L, 1);

	lua_pop(L, 1);

	GameObject* finded_obj = ObjectController::Instance()->getObject(object_name);
	obj_event finded_ev = finded_obj->getEvent();

	lua_pushinteger(L, finded_ev);

	return 1;
}

int l_set_status_on_object(lua_State* L)
{
	const char* object_name = lua_tostring(L, 1);
	unsigned int object_event = lua_tointeger(L, 2);

	lua_pop(L, 2);

	GameObject* finded_obj = ObjectController::Instance()->getObject(object_name);
	finded_obj->setEvent(static_cast<obj_event>(object_event));

	if (object_event == obj_event::STUN)
		finded_obj->setStunPreset();

	return 0;
}

int l_bindUI_On_GameObject(lua_State* L)
{
	const char* table_name = lua_tostring(L, 1);
	const char* skill_name = lua_tostring(L, 2);

	lua_pop(L, 2);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	lua_pushstring(L, "id");
	lua_gettable(L, 1);

	unsigned int add_id = lua_tointeger(L, -1);
	UIComponent* add_ui_comp = UIController::Instance()->find_UIComponent(add_id);

	ObjectController::Instance()->add_BindUI(add_ui_comp, table_name, skill_name);

	lua_pop(L, 1);

	return 0;
}

ObjectController::ObjectController(void) : selected_Object(nullptr), isScrollingEvent(false), isTouchSelected(false)
{
	CameraPos = CCPoint(0.f, 0.f);

	CCCamera* myCam = this->getCamera();
	myCam->setEyeXYZ(CameraPos.x, CameraPos.y, 0.f);
	myCam->setCenterXYZ(CameraPos.x, CameraPos.y, -0.000000000001f);
}

ObjectController::~ObjectController(void)
{
}

GameObject* ObjectController::add_Object(int obj_x, int obj_y)
{
	obj_info input_data;

	input_data.pos.x = obj_x;
	input_data.pos.y = obj_y;

	setObjectInfo(input_data);
	setGraphicFrame(input_data);
	setEffect(input_data);
	setSkill(input_data);

	GameObject* add_GameObject = new GameObject;
	add_GameObject->Init(&input_data);

	game_object_list.push_back(add_GameObject);

	return add_GameObject;
}
void ObjectController::update_Object()
{
	unsigned int game_obj_size = game_object_list.size();

	for (unsigned int i = 0; i < game_obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);
		
		if (!game_obj_iter->getIsEnemy() && game_obj_iter->getControllable())
			selected_Object = game_obj_iter;

		game_obj_iter->SetAllObjectList(game_object_list);
		game_obj_iter->Update();
	}
	setUpdateScrolling(0.0f);
}
void ObjectController::update_Dead_Object()
{
	unsigned int game_obj_size = game_object_list.size();

	auto begin = game_object_list.begin();
	auto end = game_object_list.end();
	for (unsigned int i = 0; i < game_obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);
		auto finded_iter = std::find(begin, end, game_obj_iter);

		bool isDestroy = game_obj_iter->destroy_check();

		if (isDestroy)
		{
			BD_CCLog("Index %x Object Deleted", game_obj_iter);

			GameObject* deleted_obj_address = *finded_iter;

			game_obj_iter->Destroy();
			SAFE_DELETE(game_obj_iter);
			game_object_list.erase(finded_iter);

			// 제거된 뒤에, 소거된 객체를 타겟으로 잡고 있는 케이스가 있을 수 있다.
			// 이 케이스를 막기 위하여, 모든 객체의 주소들을 검사하여, 지웠던 객체의 주소랑
			// 같을 경우, NULL 값으로 초기화를 하여, 메모리에 잘못된 접근을 막는다.
			unsigned int altered_object_size = game_object_list.size();
			for (unsigned int j = 0; j < altered_object_size; ++j)
			{
				GameObject* game_obj_iter = game_object_list.at(j);
				if (game_obj_iter->getTarget() == deleted_obj_address)
					game_obj_iter->setTarget(nullptr);
			}
			break;
		}
	}
}
void ObjectController::clear_Object()
{
	GameObject* player_obj = getObject("player");
	if (player_obj != nullptr)
	{
		obj_info player_obj_info = player_obj->getObjectInfo();
		unsigned int skill_cnt = player_obj_info.skill_list.size();
		for (unsigned int i = 0; i < skill_cnt; ++i)
		{
			skill_info* skill_iter = player_obj_info.skill_list.at(i);

			UIComponent* finded_UI = UIController::Instance()->find_UIComponent(binded_skill_name_list.at(skill_iter->skill_name));
			if (finded_UI != nullptr)
				finded_UI->setProgressTimerPercentage(0.f);
		}
	}

	map_clear(binded_ui_list);
	map_clear(binded_skill_name_list);

	unsigned int game_obj_size = game_object_list.size();

	for (unsigned int i = 0; i < game_obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);

		game_obj_iter->Destroy();
		SAFE_DELETE(game_obj_iter);
	}
	vector_clear(game_object_list);
}

void ObjectController::register_ObjectFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("add_object", l_add_Object);
	LuaCommunicator::Instance()->Register_CFunction("update_object", l_update_Object);
	LuaCommunicator::Instance()->Register_CFunction("remove_object", l_remove_Object);
	LuaCommunicator::Instance()->Register_CFunction("command_to_object", l_command_to_Object);
	LuaCommunicator::Instance()->Register_CFunction("set_ObjectInfo", l_set_ObjectInfo);
	LuaCommunicator::Instance()->Register_CFunction("set_AllObject_Event", l_set_AllObject_Event);
	LuaCommunicator::Instance()->Register_CFunction("get_status_on_object", l_get_status_on_object);
	LuaCommunicator::Instance()->Register_CFunction("set_status_on_object", l_set_status_on_object);
	LuaCommunicator::Instance()->Register_CFunction("bindUI_on_game_object", l_bindUI_On_GameObject);
}

void ObjectController::add_BindUI(UIComponent* src_ui, const char* key, const char* skill_name)
{
	CCAssert(src_ui != nullptr, "Can't BindUI To Object. UIComponent Is NULL.");
	binded_ui_list.insert(std::pair<const char*, UIComponent*>(key, src_ui));
	binded_skill_name_list.insert(std::pair<const char*, const char*>(skill_name, key));

	// 스킬이름이 공란이 아닐 경우는 버튼의 이미지를 변경해주고,
	// 스킬이름이 공란일 경우는 버튼의 이미지를 디폴트로 그대로 놔둔다.
	if (strcmp(skill_name, ""))
	{
		GameObject* player_obj = getObject("player");

		SkillController* player_skill_ctrl = player_obj->getSkillController();
		skill_info* selected_skill = player_skill_ctrl->getSkill(skill_name);

		CCSprite* skill_img_spr = CCSprite::create(selected_skill->skill_image_name);

		BD_Button* butt_inst = dynamic_cast<BD_Button*>(src_ui);
		if (butt_inst != nullptr)
		{
			CCTexture2D* skill_texture = skill_img_spr->getTexture();
			butt_inst->setButtonImage(skill_texture, skill_texture);
			skill_img_spr->retain();
		}
	}
}
void ObjectController::update_BindUI()
{
	GameObject* player_obj = getObject("player");
	SkillController* player_skill_ctrl = player_obj->getSkillController();

	auto bind_ui_begin = binded_ui_list.begin();
	auto bind_ui_end = binded_ui_list.end();

	for (auto bind_ui_iter = bind_ui_begin; bind_ui_iter != bind_ui_end; ++bind_ui_iter)
	{
		bool bind_msg_iter = static_cast<bool>(bind_ui_iter->second->send_message_main());

		if (selected_Object != nullptr)
		{
			bool isSkillSelected = false;

			auto bind_skill_begin = binded_skill_name_list.begin();
			auto bind_skill_end = binded_skill_name_list.end();

			for (auto skill_key_iter = bind_skill_begin; skill_key_iter != bind_skill_end; ++skill_key_iter)
			{
				// UI 객체에 터치 이벤트가 할당된 경우
				if (bind_msg_iter)
				{
					if (!strcmp(bind_ui_iter->first, skill_key_iter->second))
					{
						selected_Object->setSkillSelect(skill_key_iter->first);
						skill_info* selected_skill = player_skill_ctrl->getSkill(skill_key_iter->first);

						if (selected_skill != nullptr)
							isSkillSelected = true;
					}

					bind_ui_iter->second->recv_message_main(false);

					if (isSkillSelected)
					{
						selected_Object->Skill();
						break;
					}
				}
			}
		}
	}

	// 플레이어의 모든 스킬의 쿨다운 진행 정도를 스킬별 버튼에 표시해준다.
	if (player_obj != nullptr)
	{
		obj_info player_obj_info = player_obj->getObjectInfo();
		unsigned int skill_cnt = player_obj_info.skill_list.size();
		for (unsigned int i = 0; i < skill_cnt; ++i)
		{
			skill_info* skill_iter = player_obj_info.skill_list.at(i);

			UIComponent* finded_UI = UIController::Instance()->find_UIComponent(binded_skill_name_list.at(skill_iter->skill_name));
			if (finded_UI != nullptr)
			{
				if (skill_iter->isSkillUsed)
				{
					float cooldown_percentage = (skill_iter->cooldn_elapsed_time / (skill_iter->skill_cooldown * 1000)) * 100;

					finded_UI->setProgressTimerPercentage(cooldown_percentage);
				}
				else
					finded_UI->setProgressTimerPercentage(0.f);
			}
		}
	}
}

void ObjectController::setObjectInfo(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	lua_pushstring(p_lua_st, "info");
	lua_gettable(p_lua_st, 1);

	int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);
	}

	if (table_size > 0)
	{
		strncpy(p_obj_info.object_name, lua_tostring(p_lua_st, -13), sizeof(p_obj_info.object_name));
		p_obj_info.attack_point = lua_tointeger(p_lua_st, -12);
		p_obj_info.defense_point = lua_tointeger(p_lua_st, -11);
		p_obj_info.health_point = lua_tointeger(p_lua_st, -10);
		p_obj_info.recognize_area = lua_tointeger(p_lua_st, -9);
		p_obj_info.firing_area_X = lua_tointeger(p_lua_st, -8);
		p_obj_info.firing_area_Y = lua_tointeger(p_lua_st, -7);
		p_obj_info.patrol_delay_time = lua_tointeger(p_lua_st, -6);
		p_obj_info.move_speed = lua_tonumber(p_lua_st, -5);
		INTEGER_TO_BOOLEAN(p_obj_info.isEnemy, lua_toboolean(p_lua_st, -4));
		INTEGER_TO_BOOLEAN(p_obj_info.isHero, lua_toboolean(p_lua_st, -3));
		INTEGER_TO_BOOLEAN(p_obj_info.isControllable, lua_toboolean(p_lua_st, -2));
		INTEGER_TO_BOOLEAN(p_obj_info.isAreaAttack, lua_toboolean(p_lua_st, -1));

		p_obj_info.max_health_point = p_obj_info.health_point;
	}

	// info_end
	lua_pop(p_lua_st, table_size);
	// into_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setGraphicFrame(obj_info& p_obj_info)
{
	setMainImage(p_obj_info);

	setImageFrame(p_obj_info);

	setAniFrame(p_obj_info);

	setAniFrameSet(p_obj_info);
}

void ObjectController::setMainImage(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// image_table_start
	lua_pushstring(p_lua_st, "main_image");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		const char* ret_char = lua_tostring(p_lua_st, -1);
		p_obj_info.main_image.insert(std::pair<const char*, const char*>(ret_char, ret_char));
		lua_pop(p_lua_st, 1);
	}
	// image_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setImageFrame(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// image_frame_table_start
	lua_pushstring(p_lua_st, "image_frame");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		int img_table_size = LuaCommunicator::Instance()->getTableSize(3);

		for (int j = 1; j <= img_table_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		imgframe_info* img_data = new imgframe_info;
		img_data->base_image = lua_tostring(p_lua_st, -6);
		img_data->key = lua_tostring(p_lua_st, -5);
		int image_X = lua_tointeger(p_lua_st, -4), image_Y = lua_tointeger(p_lua_st, -3),
			width = lua_tointeger(p_lua_st, -2), height = lua_tointeger(p_lua_st, -1);
		img_data->in_image_area = cocos2d::CCRect(image_X, image_Y, width, height);

		p_obj_info.img_frame_list.push_back(img_data);

		BD_CCLog("image_frame = %s %s %d %d %d %d", img_data->base_image, img_data->key, image_X, image_Y, width, height);

		lua_pop(p_lua_st, img_table_size);

		lua_pop(p_lua_st, 1);
	}
	// image_frame_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setAniFrame(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// ani_image_table_start
	lua_pushstring(p_lua_st, "ani_image_frame");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);
	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		aniframe_info* aniframe_data = new aniframe_info;

		// ani_image_frame[index] table start
		int ani_table_size = LuaCommunicator::Instance()->getTableSize(3);
		const int no_table_size = 3;

		for (int j = 1; j <= no_table_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		aniframe_data->ref_image_name = lua_tostring(p_lua_st, -3);
		aniframe_data->ref_list_name = lua_tostring(p_lua_st, -2);
		aniframe_data->ani_name = lua_tostring(p_lua_st, -1);

		BD_CCLog("ani = %s %s %s", aniframe_data->ref_image_name, aniframe_data->ref_list_name, aniframe_data->ani_name);
		lua_pop(p_lua_st, no_table_size);

		///////////////////////////////////////////////
		for (int j = no_table_size + 1; j <= ani_table_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);

			int ani_frame_table_size = LuaCommunicator::Instance()->getTableSize(4);
			for (int k = 1; k <= ani_frame_table_size; ++k)
			{
				lua_pushinteger(p_lua_st, k);
				lua_gettable(p_lua_st, 4);
			}

			in_aniframe_info* in_aniframe_data = new in_aniframe_info;
			in_aniframe_data->in_list_key = lua_tostring(p_lua_st, -2);
			in_aniframe_data->frame = lua_tointeger(p_lua_st, -1);

			BD_CCLog("aniimage_frame_list = %s %d", in_aniframe_data->in_list_key, in_aniframe_data->frame);

			aniframe_data->in_aniframe_list.push_back(in_aniframe_data);

			lua_pop(p_lua_st, ani_frame_table_size);

			lua_pop(p_lua_st, 1);
		}
		lua_pop(p_lua_st, 1);

		p_obj_info.ani_image_list.push_back(aniframe_data);
	}
	// ani_image_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setAniFrameSet(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// ani_image_table_start
	lua_pushstring(p_lua_st, "ani_frame_set");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		int ani_frame_set_table_size = LuaCommunicator::Instance()->getTableSize(3);

		for (int j = 1; j <= ani_frame_set_table_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		aniframe_set_info* aniframe_set_data = new aniframe_set_info;

		aniframe_set_data->ani_name = lua_tostring(p_lua_st, -5);
		aniframe_set_data->delay = lua_tonumber(p_lua_st, -4);
		aniframe_set_data->rep_start = lua_tointeger(p_lua_st, -3);
		aniframe_set_data->rep_end = lua_tointeger(p_lua_st, -2);
		INTEGER_TO_BOOLEAN(aniframe_set_data->isOneTimePlay, lua_toboolean(p_lua_st, -1));

		p_obj_info.ani_frame_set_list.push_back(aniframe_set_data);

		BD_CCLog("aniframe_set = %s %f %d %d", aniframe_set_data->ani_name, aniframe_set_data->delay,
			aniframe_set_data->rep_start, aniframe_set_data->rep_end);

		lua_pop(p_lua_st, ani_frame_set_table_size);

		lua_pop(p_lua_st, 1);
	}
	// ani_image_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setEffect(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// ani_image_table_start
	lua_pushstring(p_lua_st, "effect_set");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		unsigned int effect_table_size = LuaCommunicator::Instance()->getTableSize(3);
		const unsigned int preset_size = 2;

		for (unsigned int j = 1; j <= preset_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		effect_info* effect_data = new effect_info;
		effect_data->effect_list_name = lua_tostring(p_lua_st, -2);
		effect_data->grap_object_key = lua_tostring(p_lua_st, -1);

		BD_CCLog("effect_data = %s %s", effect_data->effect_list_name, effect_data->grap_object_key);

		lua_pop(p_lua_st, preset_size);

		for (unsigned int j = preset_size + 1; j <= effect_table_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);

			unsigned int effect_set_size = LuaCommunicator::Instance()->getTableSize(4);
			for (unsigned int k = 1; k <= effect_set_size; ++k)
			{
				lua_pushinteger(p_lua_st, k);
				lua_gettable(p_lua_st, 4);
			}

			effect_set_info* effect_set_data = new effect_set_info;

			effect_set_data->key = lua_tostring(p_lua_st, -5);

			const char* ret_str = lua_tostring(p_lua_st, -4);
			if (!strcmp(ret_str, "draw_to_target"))
				effect_set_data->draw_type = DRAW_TO_TARGET;
			else if (!strcmp(ret_str, "draw_to_me"))
				effect_set_data->draw_type = DRAW_TO_ME;
			else if (!strcmp(ret_str, "draw_to_me_random"))
				effect_set_data->draw_type = DRAW_TO_ME_RANDOM;

			effect_set_data->relative_distance.x = lua_tointeger(p_lua_st, -3);
			effect_set_data->relative_distance.y = lua_tointeger(p_lua_st, -2);
			effect_set_data->applying_index = lua_tointeger(p_lua_st, -1);

			effect_data->effect_set_list.push_back(effect_set_data);

			BD_CCLog("effect_set_data = %s %f %f %d", effect_set_data->key, effect_set_data->relative_distance.x,
				effect_set_data->relative_distance.y, effect_set_data->applying_index);

			lua_pop(p_lua_st, effect_set_size);

			lua_pop(p_lua_st, 1);
		}

		p_obj_info.effect_list.push_back(effect_data);

		lua_pop(p_lua_st, 1);
	}
	// ani_image_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setSkill(obj_info& p_obj_info)
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// ani_image_table_start
	lua_pushstring(p_lua_st, "skill");
	lua_gettable(p_lua_st, 1);

	unsigned int skill_size = LuaCommunicator::Instance()->getTableSize(2);
	for (unsigned int i = 1; i <= skill_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		const int non_index_variable_size = 6;
		skill_info* skill_data = new skill_info;

		// Non-Index Key (Graphic Component Key)

		// chain_hit_frame start
		lua_pushstring(p_lua_st, "chain_hit_frame");
		lua_gettable(p_lua_st, 3);

		unsigned int in_skill_anim_list_size = LuaCommunicator::Instance()->getTableSize(4);
		for (unsigned int j = 1; j <= in_skill_anim_list_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 4);

			unsigned in_hit_ani_frame = lua_tointeger(p_lua_st, -1);
			skill_data->chain_hit_in_ani_frame_list.push_back(in_hit_ani_frame);
			lua_pop(p_lua_st, 1);
		}
		lua_pop(p_lua_st, 1);
		// chain_hit_frame end

		// particle_data start
		lua_pushstring(p_lua_st, "particle_data");
		lua_gettable(p_lua_st, 3);

		unsigned int in_skill_particle_size = LuaCommunicator::Instance()->getTableSize(4);
		for (unsigned int j = 1; j <= in_skill_particle_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 4);

			unsigned in_skill_particle_element_cnt = LuaCommunicator::Instance()->getTableSize(5);
			for (unsigned int k = 1; k <= in_skill_particle_element_cnt; ++k)
			{
				lua_pushinteger(p_lua_st, k);
				lua_gettable(p_lua_st, 5);
			}

			particle_data* skill_particle_data = new particle_data;
			skill_particle_data->particle_name = lua_tostring(p_lua_st, -9);
			skill_particle_data->texture_path = lua_tostring(p_lua_st, -8);
			skill_particle_data->parti_type = (particle_type)lua_tointeger(p_lua_st, -7);
			skill_particle_data->particle_system_pos.x = lua_tonumber(p_lua_st, -6);
			skill_particle_data->particle_system_pos.y = lua_tonumber(p_lua_st, -5);
			skill_particle_data->life_val = lua_tonumber(p_lua_st, -4);
			skill_particle_data->emission_rate = lua_tonumber(p_lua_st, -3);
			skill_particle_data->gravity_mass.x = lua_tonumber(p_lua_st, -2);
			skill_particle_data->gravity_mass.y = lua_tonumber(p_lua_st, -1);

			ParticleController::Instance()->add_Particle(skill_particle_data);
			skill_data->skill_particle_list.push_back(skill_particle_data);

			lua_pop(p_lua_st, in_skill_particle_element_cnt);
			lua_pop(p_lua_st, 1);
		}
		lua_pop(p_lua_st, 1);
		// particle_data end
		
		lua_pushstring(p_lua_st, "global_vibration");
		lua_gettable(p_lua_st, 3);
		skill_data->global_vibration = lua_tonumber(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		lua_pushstring(p_lua_st, "charge_dist");
		lua_gettable(p_lua_st, 3);
		skill_data->charge_dist = lua_tointeger(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		lua_pushstring(p_lua_st, "power_percent");
		lua_gettable(p_lua_st, 3);
		skill_data->power_percentage = lua_tonumber(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		lua_pushstring(p_lua_st, "skill_cooldown");
		lua_gettable(p_lua_st, 3);
		skill_data->skill_cooldown = lua_tonumber(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		// Non-Index Key End(Graphic Component Key)

		// Index Key
		unsigned int in_skill_content_size = LuaCommunicator::Instance()->getTableSize(3);
		for (unsigned int j = 1; j <= in_skill_content_size - non_index_variable_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		skill_data->skill_image_name = lua_tostring(p_lua_st, -5);
		skill_data->ref_ani_name = lua_tostring(p_lua_st, -4);
		skill_data->skill_name = lua_tostring(p_lua_st, -3);
		skill_data->attack_type = lua_tostring(p_lua_st, -2);
		skill_data->skill_type = lua_tostring(p_lua_st, -1);

		lua_pop(p_lua_st, in_skill_content_size - non_index_variable_size);

		skill_data->cooldn_start_time = 0;
		skill_data->cooldn_end_time = 0;
		skill_data->cooldn_elapsed_time = 0;
		skill_data->isSkillUsed = false;

		BD_CCLog("%s %s %s %s %s %d", skill_data->skill_image_name, skill_data->ref_ani_name, skill_data->skill_name, skill_data->attack_type, skill_data->skill_type,
			skill_data->chain_hit_in_ani_frame_list.size());

		p_obj_info.skill_list.push_back(skill_data);

		lua_pop(p_lua_st, 1);
	}
	lua_pop(p_lua_st, 1);
}

void ObjectController::setUpdateScrolling(float delta_x)
{
	unsigned int obj_size = game_object_list.size();
	
	bool isCameraFixed = SceneManager::Instance()->getIsCameraFixed();
	CCSize win_size = CCDirector::sharedDirector()->getWinSize();
	CCPoint map_start_pt = Map::Instance()->getMapStartPoint();
	CCPoint map_end_pt = Map::Instance()->getMapEndPoint();

	if (!isCameraFixed)
	{
		if (CameraPos.x >= map_start_pt.x && CameraPos.x + win_size.width <= map_end_pt.x)
			CameraPos.x += delta_x;
		else if (CameraPos.x < map_start_pt.x)
			CameraPos.x = 0.f;
		else if (CameraPos.x + win_size.width > map_end_pt.x)
			CameraPos.x = map_end_pt.x - win_size.width;
	}
	else
	{
		GameObject* player_obj = getObject("player");

		if (player_obj != nullptr)
		{
			CCPoint player_pos = player_obj->getObjectPos();
			CameraPos.x = player_pos.x - (win_size.width / 2);
		}

		if (CameraPos.x < map_start_pt.x)
			CameraPos.x = 0.f;
		else if (CameraPos.x + win_size.width > map_end_pt.x)
			CameraPos.x = map_end_pt.x - win_size.width;
	}

	CCCamera* myCam = this->getCamera();
	myCam->setEyeXYZ(CameraPos.x, CameraPos.y, 0.f);
	myCam->setCenterXYZ(CameraPos.x, CameraPos.y, -0.1f);
}

void ObjectController::setAllObjectEvent(obj_event obj_ev)
{
	unsigned int obj_size = game_object_list.size();
	for (unsigned i = 0; i < obj_size; ++i)
	{
		GameObject* game_pt = game_object_list.at(i);

		game_pt->setEvent(obj_ev);
	}
}

bool ObjectController::getIsObjectDead(const char* object_name)
{
	bool isObjectDead = false;

	unsigned int obj_size = game_object_list.size();

	for (unsigned int i = 0; i < obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);

		if (game_obj_iter->dead_check())
		{
			obj_info obj_info_inst = game_obj_iter->getObjectInfo();
			if (!strcmp(obj_info_inst.object_name, object_name))
			{
				isObjectDead = true;
				break;
			}
		}
	}

	return isObjectDead;
}
unsigned int ObjectController::getEnemyObjectCnt()
{
	unsigned int ret_cnt = 0;

	unsigned int obj_size = game_object_list.size();

	for (unsigned int i = 0; i < obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);
		if (game_obj_iter->getIsEnemy())
			++ret_cnt;
	}

	return ret_cnt;
}
unsigned int ObjectController::getAllyObjectCnt()
{
	unsigned int ret_cnt = 0;

	unsigned int obj_size = game_object_list.size();

	for (unsigned int i = 0; i < obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);

		if (!game_obj_iter->getIsEnemy())
			++ret_cnt;
	}

	return ret_cnt;
}
const char* ObjectController::getCollidedTrigger(const char* obj_name)
{
	const char* ret_str = nullptr;

	if (obj_name != nullptr)
	{
		GameObject* player_obj = getObject(obj_name);

		if (player_obj != nullptr)
			ret_str = Map::Instance()->get_collided_trigger(player_obj->getObjectPos());
	}

	return ret_str;
}
void ObjectController::setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}
void ObjectController::setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	isScrollingEvent = true;
}
void ObjectController::setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	CCPoint converted_pt = pTouch->getLocation();
	converted_pt.x += CameraPos.x;

	if (!isScrollingEvent)
	{
		bool isEqualSelected = false;
		bool isEnemySelected = false;
		bool isObjectSelected = false;
		unsigned int object_size = game_object_list.size();

		GameObject* enemy_iter = nullptr;
		for (unsigned int i = 0; i < object_size; ++i)
		{
			GameObject* obj_iter = game_object_list.at(i);
			cocos2d::CCRect obj_rect = obj_iter->getObjectRect(converted_pt);

			if (!obj_iter->dead_check())
			{
				if (obj_rect.containsPoint(converted_pt))
				{
					if (!obj_iter->getIsEnemy())
					{
						isEnemySelected = false;
						if (obj_iter->getControllable())
						{
							if (selected_Object != obj_iter)
							{
								if (selected_Object != nullptr)
									selected_Object->setIsSelected(false);

								obj_iter->setIsSelected(true);
								selected_Object = obj_iter;
								isTouchSelected = true;
							}
							else
								isEqualSelected = true;
						}
						else
							obj_iter->setIsSelected(false);
					}
					else
					{
						isEnemySelected = true;
						enemy_iter = obj_iter;
					}

					isObjectSelected = true;
					break;
				}
			}
		}

		if (selected_Object != nullptr)
		{
			if (!isObjectSelected)
			{
				if (selected_Object->getControllable())
					selected_Object->Move(converted_pt);
			}
			else
			{
				if (isEqualSelected)
					selected_Object->Move(converted_pt);
				else if (isEnemySelected)
					selected_Object->Attack(enemy_iter);
			}
		}

		GameObject* player_obj = getObject("player");
		if (player_obj != nullptr)
		{
			obj_event player_ev = player_obj->getEvent();
			switch (player_ev)
			{
			case STUN:
				player_obj->addTapCount();
				break;
			}
		}
	}

	isScrollingEvent = false;
}