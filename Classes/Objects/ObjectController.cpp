#include "ObjectController.h"
#include "../LuaCommunicator.h"
#include "../Graphics/GraphicsController.h"
#include "../UI/UIController.h"
#include "../Utility/Utility.h"
#include "ParticleController.h"
#include "Map.h"

USING_NS_CC;

int l_add_Object(lua_State* L)
{
	ObjectController::Instance()->setLuaState(L);

	const char *table_name = nullptr;

	table_name = lua_tostring(L, 1);
	int posX = lua_tointeger(L, 2);
	int posY = lua_tointeger(L, 3);

	lua_pop(L, 3);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	GameObject* created_obj = ObjectController::Instance()->add_Object(posX, posY);

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
	}
	else if (!strcmp(command_str, "Skill"))
	{
		
	}
	else if (!strcmp(command_str, "Patrol"))
	{
		int pos_X = lua_tointeger(L, 3);
		int pos_Y = lua_tointeger(L, 4);

		CCPoint dest_pt = CCPoint(pos_X, pos_Y);

		finded_obj->Patrol(dest_pt);
	}

	return 0;
}

int l_bindUI_On_GameObject(lua_State* L)
{
	const char* table_name = lua_tostring(L, -2);
	const char* ui_key = lua_tostring(L, -1);

	lua_pop(L, 2);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	lua_pushstring(L, "id");
	lua_gettable(L, 1);

	unsigned int add_id = lua_tointeger(L, -1);
	UIComponent* add_ui_comp = UIController::Instance()->find_UIComponent(add_id);

	ObjectController::Instance()->add_BindUI(add_ui_comp, ui_key);

	lua_pop(L, 1);

	return 0;
}

ObjectController::ObjectController(void) : selected_Object(nullptr), isScrollingEvent(false), isTouchSelected(false)
{
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

		bool isDead = game_obj_iter->dead_check();
		bool isDestroy = game_obj_iter->destroy_check();

		if (isDead)
		{
			// 현재 선택되어 있던 객체가 죽게 될 경우,
			// 죽어가는 객체가 선택되어져 있는 상태라면
			// 메모리를 잘못 가르킬 수 있으므로, 커서의 참조를 없앤다.
			if (selected_Object == game_obj_iter)
				selected_Object = nullptr;
		}

		if (isDestroy)
		{
			if (selected_Object == game_obj_iter)
				selected_Object = nullptr;

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
	map_clear(binded_ui_list);

	unsigned int game_obj_size = game_object_list.size();

	for (unsigned int i = 0; i < game_obj_size; ++i)
	{
		GameObject* game_obj_iter = game_object_list.at(i);

		game_obj_iter->Destroy();
		SAFE_DELETE(game_obj_iter);
	}
	vector_clear(game_object_list);

	selected_Object = nullptr;
}

void ObjectController::register_ObjectFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("add_object", l_add_Object);
	LuaCommunicator::Instance()->Register_CFunction("update_object", l_update_Object);
	LuaCommunicator::Instance()->Register_CFunction("remove_object", l_remove_Object);
	LuaCommunicator::Instance()->Register_CFunction("command_to_object", l_command_to_Object);
	LuaCommunicator::Instance()->Register_CFunction("bindUI_on_game_object", l_bindUI_On_GameObject);
}

void ObjectController::add_BindUI(UIComponent* src_ui, const char* key)
{
	CCAssert(src_ui != nullptr, "Can't BindUI To Object. UIComponent Is NULL.");
	binded_ui_list[key] = src_ui;
}
void ObjectController::update_BindUI()
{
	auto begin = binded_ui_list.begin();
	auto end = binded_ui_list.end();

	UIComponent* skill_iter = nullptr, *object_iter = nullptr;

	for (; begin != end; ++begin)
	{
		if (!strncmp(begin->first, "skill", sizeof(begin->first)))
			skill_iter = begin->second;
		else if (!strncmp(begin->first, "object", sizeof(begin->first)))
			object_iter = begin->second;
	}

	// 스킬 UI 셋팅
	if (selected_Object != nullptr)
	{
		if (!selected_Object->dead_check())
		{
			std::vector<cocos2d::CCSprite*> list_pt = selected_Object->getSkillIconList();
			slidesel_msg_recv msg;

			msg.list_size = list_pt.size();
			msg.current_index = -1;
			skill_iter->recv_message_main((void*)&msg);

			unsigned int skill_selected_index = skill_iter->get_Message();
			update_Skill_UI(skill_selected_index, skill_iter);

			if (!skill_iter->getIsSwipeMode())
			{
				skill_iter->setIsSwipeMode(true);
				selected_Object->Skill();
			}
		}
	}

	// 오브젝트 선택 UI 셋팅
	slidesel_msg_recv msg;
	unsigned int object_selected_index = object_iter->get_Message();
	update_Object_UI(object_selected_index, object_iter);

	msg.list_size = game_object_list.size();
	msg.current_index = object_selected_index;
	object_iter->recv_message_main((void*)&msg);
}

void ObjectController::setObjectInfo(obj_info& p_obj_info)
{
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
		p_obj_info.partol_delay_time = lua_tointeger(p_lua_st, -6);
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
	// image_table_start
	lua_pushstring(p_lua_st, "main_image");
	lua_gettable(p_lua_st, 1);

	unsigned int table_size = LuaCommunicator::Instance()->getTableSize(2);

	for (unsigned int i = 1; i <= table_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		const char* ret_char = lua_tostring(p_lua_st, -1);
		p_obj_info.main_image[ret_char] = ret_char;
		lua_pop(p_lua_st, 1);
	}
	// image_table_end
	lua_pop(p_lua_st, 1);
}
void ObjectController::setImageFrame(obj_info& p_obj_info)
{
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
		int frame_size = ani_table_size - no_table_size;

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
			effect_set_data->draw_type = (!strcmp(lua_tostring(p_lua_st, -4), "draw_to_me")) ? DRAW_TO_ME : DRAW_TO_TARGET;
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
	// ani_image_table_start
	lua_pushstring(p_lua_st, "skill");
	lua_gettable(p_lua_st, 1);

	unsigned int skill_size = LuaCommunicator::Instance()->getTableSize(2);
	for (unsigned int i = 1; i <= skill_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		const int non_index_variable_size = 4;
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

		// Non-Index Key
		lua_pushstring(p_lua_st, "charge_dist");
		lua_gettable(p_lua_st, 3);
		skill_data->charge_dist = lua_tointeger(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		lua_pushstring(p_lua_st, "power_percent");
		lua_gettable(p_lua_st, 3);
		skill_data->power_percentage = lua_tonumber(p_lua_st, -1);
		lua_pop(p_lua_st, 1);

		BD_CCLog("%d %f", skill_data->charge_dist, skill_data->power_percentage);

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

		BD_CCLog("%s %s %s %s %s %d", skill_data->skill_image_name, skill_data->ref_ani_name, skill_data->skill_name, skill_data->attack_type, skill_data->skill_type,
			skill_data->chain_hit_in_ani_frame_list.size());

		p_obj_info.skill_list.push_back(skill_data);

		lua_pop(p_lua_st, 1);
	}
	lua_pop(p_lua_st, 1);
}
void ObjectController::update_Skill_UI(unsigned int index, UIComponent* current_ui)
{
	selected_Object->setSkillIconSelect(index);

	slidesel_msg_send* send_slide_sel_msg = (slidesel_msg_send*)current_ui->send_message_main();

	unsigned int game_obj_size = game_object_list.size();
	for (unsigned int i = 0; i < game_obj_size; ++i)
	{
		GameObject* game_iter = game_object_list.at(i);

		auto skill_list = game_iter->getSkillIconList();
		unsigned int selected_skill_size = skill_list.size();
		for (unsigned int j = 0; j < selected_skill_size; ++j)
		{
			cocos2d::CCSprite* skill_spr_iter = skill_list[j];

			skill_spr_iter->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
			skill_spr_iter->setPosition(send_slide_sel_msg->linked_element_pos);
		}
	}

	SAFE_DELETE(send_slide_sel_msg);
}
void ObjectController::update_Object_UI(unsigned int& index, UIComponent* current_ui)
{
	unsigned int obj_size = game_object_list.size();

	if (selected_Object != nullptr)
	{
		slidesel_msg_send* send_slide_sel_msg = (slidesel_msg_send*)current_ui->send_message_main();
		// 터치로 객체가 선택됬을 경우, 그 객체를 찾아서 SlideSelector의 index를 유지시켜줘야한다.
		// 그래야 SlideSelector의 인덱스에 영향을 받지 않고, 선택될 수 있기 때문이다.
		// 터치를 통해 선택되지 않았을 경우, SlideSelector를 통하여 객체를 선택할 수 있게 된다.
		if (isTouchSelected)
		{
			for (unsigned int i = 0; i < obj_size; ++i)
			{
				GameObject* obj_iter = game_object_list.at(i);

				if (obj_iter == selected_Object)
				{
					selected_Object = obj_iter;
					index = i;
					break;
				}
			}

			isTouchSelected = false;
		}
		else
		{
			GameObject* finded_iter = game_object_list.at(index);
			if (!finded_iter->dead_check())
			{
				if (finded_iter != selected_Object)
				{
					for (unsigned int i = 0; i < obj_size; ++i)
					{
						GameObject* obj_iter = game_object_list.at(i);
						if (finded_iter == obj_iter)
						{
							if (!finded_iter->getIsEnemy())
							{
								selected_Object = obj_iter;
								obj_iter->setIsSelected(true);
							}
							else
							{
								cocos2d::CCPoint slide_direction = send_slide_sel_msg->slide_direction_vec;

								// Index를 갱신 시킴과 동시에, 이전에 선택되었던 아군 객체로 돌아갔으므로,
								// 선택된 객체의 선택 표식을 다시 켜줘야만 한다.

								// UI객체로부터 어느 방향으로 슬라이드 이벤트를 일으켰나를 확인.
								// 방향에 따라서 index를 관리해 준다.
								if (send_slide_sel_msg->isVertical)
								{
									if (slide_direction.y > 0)
									{
										selected_Object->setIsSelected(true);
										++index;
									}
									else if (slide_direction.y < 0)
									{
										selected_Object->setIsSelected(true);
										--index;
									}
								}
								else
								{
									if (slide_direction.x > 0)
									{
										selected_Object->setIsSelected(true);
										++index;
									}
									else if (slide_direction.x < 0)
									{
										selected_Object->setIsSelected(true);
										--index;
									}
								}
								obj_iter->setIsSelected(false);
							}
						}
						else
							obj_iter->setIsSelected(false);
					}
				}
				else
				{
				}
			}
		}
		SAFE_DELETE(send_slide_sel_msg);
	}
	else
	{
		GameObject* selected_obj_iter = game_object_list.at(index);
		if (!selected_obj_iter->getIsEnemy())
		{
			selected_Object = game_object_list.at(index);

			selected_Object->setIsSelected(true);
		}
	}
}
void ObjectController::setUpdateScrolling(float delta_x)
{
	unsigned int obj_size = game_object_list.size();
	
	bool isCameraFixed = SceneManager::Instance()->getIsCameraFixed();
	bool isCameraReset = SceneManager::Instance()->getIsCameraReset();

	/* 카메라의 고정된 상태와 고정되지 않은 상태에 따라서 상태를 나누어 좌표를 다시 갱신해줘야 한다. 
	   실제적으로 카메라의 동작이 아니고, 카메라의 동작을 흉내냈기 때문. (2D 좌표계에서 카메라를 만지면
	   Projection이 꼬일 수 있다. Translate만 하면 원래 행렬에 영향 안줘서 화면을 움직일 수 있는걸로 알고 있으나,
	   정확하게 아는게 아니고, 정보가 너무 적은 관계상 위와 같이 좌표 갱신으로 구현했다.) */
	if (!isCameraFixed)
	 {
		/* 카메라 고정 => 비고정 상태로 처음 전환할 때(게임 최초 시작의 상태이다.
						  정확히는 비고정 상태로 전환이 아니고 비고정 상태이다.) */
		if (!isCameraReset)
		{
			if (Map::Instance()->getIsCanScrolling())
			{
				for (unsigned int i = 0; i < obj_size; ++i)
				{
					GameObject* obj_iter = game_object_list.at(i);

					GameObject* obj_iter_target = obj_iter->getTarget();

					CCPoint obj_pos = obj_iter->getObjectPos();
					CCPoint obj_dest_pos = obj_iter->getDestMovePos();
					float moved_delta_x = obj_iter->getMovedDeltaX();

					obj_dest_pos.x += moved_delta_x;

					obj_event obj_ev = obj_iter->getEvent();

					switch (obj_ev)
					{
					case MOVE:
						obj_iter->Move(obj_dest_pos);
						obj_iter->change_PosList_On_Scrolling(delta_x);
						break;
					case ATTACK:
						obj_iter->Attack(obj_iter_target);
						break;
					case SKILL:
						obj_iter->Skill();
						break;
					case PATROL:
						obj_iter->change_PosList_On_Scrolling(delta_x);
						break;
					default:
						break;
					}

					obj_iter->change_ThrowPosList_On_Scrolling(delta_x);
					obj_iter->change_SkillPos_On_Scrolling(delta_x);
				}
			}
			else
				Map::Instance()->reset_MapCoordinate();

			SceneManager::Instance()->setIsCameraReset(true);
		}
		/* 카메라 고정 => 비고정 상태로 전환이 완료되었고 유지되는 상태 */
		else
		{
			if (Map::Instance()->getIsCanScrolling())
			{
				for (unsigned int i = 0; i < obj_size; ++i)
				{
					GameObject* obj_iter = game_object_list.at(i);
					CCPoint obj_pos = obj_iter->getObjectPos();

					obj_event obj_ev = obj_iter->getEvent();

					// 이동 명령시에는, 리스트를 직접 제작하여, 그에 맞춰 움직이지만,
					// 공격 명령시에는, 타겟을 계속 쫓아가야하므로 실시간으로 계산을 하게 된다.
					// 두 케이스는 스크롤시 처리 방법이 다르기 때문에, 케이스로 나눈다.
					switch (obj_ev)
					{
					case MOVE:
						obj_iter->change_PosList_On_Scrolling(delta_x);
						break;
					case PATROL:
						obj_pos.x += delta_x;
						obj_iter->change_PosList_On_Scrolling(delta_x);
						break;
					default:
						obj_pos.x += delta_x;
						obj_iter->setMovedDeltaX(0.0f);
						obj_iter->setObjectPos(obj_pos);
						break;
					}

					obj_iter->setObjectPos(obj_pos);
					obj_iter->change_ThrowPosList_On_Scrolling(delta_x);
					obj_iter->change_SkillPos_On_Scrolling(delta_x);
				}
			}
			else
				Map::Instance()->reset_MapCoordinate();
		}
	}
	else
	{
		for (unsigned int i = 0; i < obj_size; ++i)
		{
			GameObject* obj_iter = game_object_list.at(i);
			CCPoint obj_pos = obj_iter->getObjectPos();

			obj_event obj_ev = obj_iter->getEvent();

			if (!obj_iter->getIsEnemy())
			{
				CCSize win_size = CCDirector::sharedDirector()->getWinSize();
				
				// 플레이어 및 나머지 객체를 가운데로 고정시켜놓는다.
				bool isCanMapScrolling = Map::Instance()->getIsCanScrolling();
				float dist_x_to_screen_mid_x = 0.0f, move_speed = 0.0f, screen_mid_x = win_size.width / 2;
				if (isCanMapScrolling)
				{
					dist_x_to_screen_mid_x = screen_mid_x - obj_pos.x;
					obj_pos.x = screen_mid_x;
				}
				else
				{
					CCPoint map_start_pt = Map::Instance()->getMapStartPoint();
					CCPoint map_end_pt = Map::Instance()->getMapEndPoint();

					if (obj_pos.x >= screen_mid_x && map_end_pt.x > win_size.width)
					{
						dist_x_to_screen_mid_x = screen_mid_x - obj_pos.x;
						obj_pos.x = screen_mid_x;
					}
					else if (obj_pos.x < screen_mid_x && map_end_pt.x <= win_size.width)
					{
						dist_x_to_screen_mid_x = screen_mid_x - obj_pos.x;
						obj_pos.x = screen_mid_x;
					}
				}

				obj_iter->setObjectPos(obj_pos);
				obj_iter->change_PosList_On_Scrolling(dist_x_to_screen_mid_x);
				obj_iter->change_ThrowPosList_On_Scrolling(dist_x_to_screen_mid_x);
				obj_iter->change_SkillPos_On_Scrolling(dist_x_to_screen_mid_x);

				Map::Instance()->setUpdateScrolling(dist_x_to_screen_mid_x);
				for (unsigned int j = 0; j < obj_size; ++j)
				{
					GameObject* other_obj_iter = game_object_list.at(j);
					CCPoint other_obj_pos = other_obj_iter->getObjectPos();

					if (other_obj_iter != obj_iter)
					{
						other_obj_pos.x += dist_x_to_screen_mid_x;
						other_obj_iter->change_PosList_On_Scrolling(dist_x_to_screen_mid_x);
						other_obj_iter->change_ThrowPosList_On_Scrolling(dist_x_to_screen_mid_x);
						other_obj_iter->setObjectPos(other_obj_pos);
					}
				}

				// 고정시킨 뒤에 나머지 객체들을 움직이는 속도에 따라서 바꿔준다.
				if (obj_iter->getIsLeft())
					move_speed = obj_iter->getMoveSpeed();
				else
					move_speed = -obj_iter->getMoveSpeed();

				skill_info* curr_skill = obj_iter->getSkillController()->getCurrentSkill();
				switch (obj_ev)
				{
				case MOVE:
				case PATROL:
					obj_iter->addMovedDeltaX(move_speed);
					break;
				case ATTACK:
					if (obj_iter->getIsCollidedToTarget())
						move_speed = 0.0f;

					obj_iter->setMovedDeltaX(move_speed);
					break;
				case SKILL:
					if (!strcmp(curr_skill->skill_type, "Charge"))
						move_speed *= charge_speed;
					else
						move_speed = 0.f;

					obj_iter->setMovedDeltaX(move_speed);
					break;
				case SEARCHING_RECOGNIZE_AREA:
					move_speed = 0.0f;
					obj_iter->setMovedDeltaX(move_speed);
					break;
				default:
					move_speed = 0.0f;
					obj_iter->setMovedDeltaX(move_speed);
					break;
				}

				if (isCanMapScrolling)
				{
					obj_iter->change_PosList_On_Scrolling(move_speed);
					obj_iter->change_ThrowPosList_On_Scrolling(move_speed);
					obj_iter->change_SkillPos_On_Scrolling(move_speed);

					Map::Instance()->setUpdateScrolling(move_speed);
					for (unsigned int j = 0; j < obj_size; ++j)
					{
						GameObject* other_obj_iter = game_object_list.at(j);
						CCPoint other_obj_pos = other_obj_iter->getObjectPos();

						if (other_obj_iter != obj_iter)
						{
							other_obj_pos.x += move_speed;
							other_obj_iter->change_ThrowPosList_On_Scrolling(move_speed);
							other_obj_iter->change_PosList_On_Scrolling(move_speed);
							other_obj_iter->change_SkillPos_On_Scrolling(move_speed);
							other_obj_iter->setObjectPos(other_obj_pos);
						}
					}
				}
				break;
			}
		}
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
			cocos2d::CCRect obj_rect = obj_iter->getObjectRect(pTouch->getLocation());

			if (!obj_iter->dead_check())
			{
				if (obj_rect.containsPoint(pTouch->getLocation()))
				{
					if (!obj_iter->getIsEnemy())
					{
						isEnemySelected = false;
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
					selected_Object->Move(pTouch->getLocation());
			}
			else
			{
				if (isEqualSelected)
					selected_Object->Move(pTouch->getLocation());
				else if (isEnemySelected)
					selected_Object->Attack(enemy_iter);
			}
		}
	}

	isScrollingEvent = false;
}