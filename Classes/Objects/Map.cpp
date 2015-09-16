#include "Map.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"
#include "ParticleController.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"
#include "../StageManager.h"

USING_NS_CC;

#include <algorithm>

int l_add_Map(lua_State* L)
{
	const char *table_name = nullptr;

	table_name = lua_tostring(L, 1);

	lua_pop(L, 1);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	Map::Instance()->Init();

	return 0;
}

int l_is_map_scrolled(lua_State* L)
{
	int scrolling_val = Map::Instance()->getScrollingValue();
	bool isMapScrolled = (abs(scrolling_val) > swipe_disable_range) ? true : false;

	lua_pushboolean(L, isMapScrolled);

	return 1;
}

bool sort_map_index(map_piece* fir, map_piece* sec)
{
	return fir->map_index < sec->map_index;
}

Map::Map(void) : in_map_particle_spr(nullptr), trigger_draw_node(nullptr), scrolling_val(0)
{
}

Map::~Map(void)
{
}

void Map::Init()
{
	set_In_Map_Object_Resource_Info();

	set_Map_Info();

	sort_Map_On_Index();

	set_In_Map_Particle();
}
void Map::Update()
{
	
}
void Map::Destroy()
{
	unsigned int map_piece_size = map_piece_list.size();

	for(unsigned int i = 0; i < map_piece_size; ++i)
	{
		map_piece* map_piece_iter = map_piece_list.at(i);
		
		unsigned int in_map_object_size = map_piece_iter->in_map_obj_list.size();
		for(unsigned int j = 0; j < in_map_object_size; ++j)
		{
			in_map_obj* in_map_obj_iter = map_piece_iter->in_map_obj_list.at(j);

			ObjectController::Instance()->removeChild(in_map_obj_iter->obj_spr, true);

			SAFE_DELETE(in_map_obj_iter);
		}
		vector_clear(map_piece_iter->in_map_obj_list);

		ObjectController::Instance()->removeChild(map_piece_iter->background_image, true);

		SAFE_DELETE(map_piece_iter);
	}
	vector_clear(map_piece_list);
	map_clear(trigger_list);

	if (in_map_particle_spr != nullptr)
	{
		in_map_particle_spr->removeAllChildrenWithCleanup(true);

		in_map_particle_spr = nullptr;

		ParticleController::Instance()->destroy_Particle();
	}

	if (trigger_draw_node != nullptr)
	{
		trigger_draw_node->removeFromParentAndCleanup(true);

		trigger_draw_node = nullptr;
	}
}
void Map::register_MapFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("add_map", l_add_Map);
	LuaCommunicator::Instance()->Register_CFunction("is_map_scrolled", l_is_map_scrolled);
}
void Map::setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}
void Map::setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}
void Map::setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}
bool Map::isInMovingArea(cocos2d::CCPoint src_pt)
{
	cocos2d::CCRect can_move_Rect;

	cocos2d::CCPoint start_pt = getMapStartPoint();
	cocos2d::CCPoint end_pt = getMapEndPoint();

	bool isCanMove = false;
	unsigned int map_size = map_piece_list.size();
	for(unsigned int i = 0; i < map_size; ++i)
	{
		map_piece* map_piece_iter = map_piece_list.at(i);
		cocos2d::CCPoint pt = map_piece_iter->background_image->getPosition();
		float width = map_piece_iter->moving_area.size.width, height = map_piece_iter->moving_area.size.height;
		can_move_Rect = cocos2d::CCRect(pt.x, 0.0f, width, height);

		if(can_move_Rect.containsPoint(src_pt))
			isCanMove = true;
	}

	return isCanMove;
}
const char* Map::get_collided_trigger(cocos2d::CCPoint src_pt)
{
	const char* ret_str = nullptr;

	auto trig_begin = trigger_list.begin();
	auto trig_end = trigger_list.end();

	for (auto j = trig_begin; j != trig_end; ++j)
	{
		if (!j->second->isTriggered)
		{
			if (j->second->trigger_rect.containsPoint(src_pt))
			{
				j->second->isTriggered = true;
				ret_str = j->first;
				break;
			}
		}
	}

	return ret_str;
}
void Map::set_Map_Info()
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	const int constant_data_size = 3;
	unsigned int map_piece_size = LuaCommunicator::Instance()->getTableSize(1);

	// 맵의 실제 정보로부터 시작한다. 1번째는 맵에 들어갈 오브젝트들을 레퍼런스할 plist가 들어가 있다.
	for (unsigned int i = 2; i <= map_piece_size - constant_data_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 1);

		map_piece_info map_piece_data;

		// start Preset Info
		lua_pushstring(p_lua_st, "index");
		lua_gettable(p_lua_st, 2);

		lua_pushstring(p_lua_st, "background");
		lua_gettable(p_lua_st, 2);

		map_piece_data.map_index = lua_tointeger(p_lua_st, -2);
		map_piece_data.background_image_name = lua_tostring(p_lua_st, -1);

		lua_pop(p_lua_st, 2);

		lua_pushstring(p_lua_st, "move_area");
		lua_gettable(p_lua_st, 2);

		unsigned int move_area_element_size = LuaCommunicator::Instance()->getTableSize(3);
		for(unsigned int j = 1; j <= move_area_element_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}
		
		int x, y, width, height;
		x = lua_tointeger(p_lua_st, -4);
		y = lua_tointeger(p_lua_st, -3);
		width = lua_tointeger(p_lua_st, -2);
		height = lua_tointeger(p_lua_st, -1);

		map_piece_data.moving_area = cocos2d::CCRect(x, y, width, height);

		lua_pop(p_lua_st, move_area_element_size);

		lua_pop(p_lua_st, 1);

		BD_CCLog("%d %s %d %d %d %d", map_piece_data.map_index, map_piece_data.background_image_name, x, y, width, height);

		// start In Map Object Info
		unsigned int in_map_object_size = LuaCommunicator::Instance()->getTableSize(2);
		for(unsigned int j = 1; j <= in_map_object_size - constant_data_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 2);
			
			unsigned int in_map_element_size = LuaCommunicator::Instance()->getTableSize(3);
			for(unsigned int k = 1; k <= in_map_element_size; ++k)
			{
				lua_pushinteger(p_lua_st, k);
				lua_gettable(p_lua_st, 3);
			}

			in_map_info* in_map_obj = new in_map_info;

			in_map_obj->obj_pos.x = lua_tonumber(p_lua_st, -5);
			in_map_obj->obj_pos.y = lua_tonumber(p_lua_st, -4);
			in_map_obj->ref_list = lua_tostring(p_lua_st, -3);
			in_map_obj->ref_key = lua_tostring(p_lua_st, -2);
			in_map_obj->in_map_obj_type = (map_obj_type)lua_tointeger(p_lua_st, -1);

			BD_CCLog("%f %f %s %s", in_map_obj->obj_pos.x, in_map_obj->obj_pos.y, in_map_obj->ref_list, in_map_obj->ref_key);

			map_piece_data.in_map_info_list.push_back(in_map_obj);

			lua_pop(p_lua_st, in_map_element_size);

			// end In Map Object Info
			lua_pop(p_lua_st, 1);
		}
		lua_pop(p_lua_st, 1);

		lua_pushstring(p_lua_st, "trigger_data");
		lua_gettable(p_lua_st, 1);

		unsigned int map_trigger_cnt = LuaCommunicator::Instance()->getTableSize(2);

		for (unsigned int i = 1; i <= map_trigger_cnt; ++i)
		{
			lua_pushinteger(p_lua_st, i);
			lua_gettable(p_lua_st, 2);

			unsigned int map_trigger_attri_cnt = LuaCommunicator::Instance()->getTableSize(3);
			for (unsigned int j = 1; j <= map_trigger_attri_cnt; ++j)
			{
				lua_pushinteger(p_lua_st, j);
				lua_gettable(p_lua_st, 3);
			}

			int x = 0, y = 0, width = 0, height = 0;

			x = lua_tointeger(p_lua_st, -6);
			y = lua_tointeger(p_lua_st, -5);
			width = lua_tointeger(p_lua_st, -4);
			height = lua_tointeger(p_lua_st, -3);
			const char* trigger_type = lua_tostring(p_lua_st, -2);
			const char* trigger_name = lua_tostring(p_lua_st, -1);

			map_trig_info* add_trigger_info = new map_trig_info;

			add_trigger_info->isTriggered = false;
			add_trigger_info->trigger_type = trigger_type;
			add_trigger_info->trigger_pos = CCPoint(x, y);
			add_trigger_info->trigger_rect = CCRect(x, y, width, height);

			map_piece_data.trigger_list.insert(std::pair<const char*, map_trig_info*>(trigger_name, add_trigger_info));

			lua_pop(p_lua_st, map_trigger_attri_cnt);

			lua_pop(p_lua_st, 1);
		}

		set_Map(map_piece_data);

		unsigned int in_obj_size = map_piece_data.in_map_info_list.size();

		for(unsigned int j = 0; j < in_obj_size; ++j)
			SAFE_DELETE(map_piece_data.in_map_info_list.at(j));
		
		map_clear(map_piece_data.trigger_list);
		vector_clear(map_piece_data.in_map_info_list);

		lua_pop(p_lua_st, 1);
	}

	lua_pushstring(p_lua_st, "particle_data");
	lua_gettable(p_lua_st, 1);

	unsigned int map_particle_size = LuaCommunicator::Instance()->getTableSize(2);
	for (unsigned int i = 1; i <= map_particle_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		unsigned int map_particle_attri_size = LuaCommunicator::Instance()->getTableSize(3);
		for (unsigned int j = 1; j <= map_particle_attri_size; ++j)
		{
			lua_pushinteger(p_lua_st, j);
			lua_gettable(p_lua_st, 3);
		}

		particle_data* add_parti_data = new particle_data;

		add_parti_data->particle_name = lua_tostring(p_lua_st, -9);
		add_parti_data->texture_path = lua_tostring(p_lua_st, -8);
		add_parti_data->parti_type = (particle_type)lua_tointeger(p_lua_st, -7);
		add_parti_data->particle_system_pos.x = lua_tonumber(p_lua_st, -6);
		add_parti_data->particle_system_pos.y = lua_tonumber(p_lua_st, -5);
		add_parti_data->life_val = lua_tonumber(p_lua_st, -4);
		add_parti_data->emission_rate = lua_tonumber(p_lua_st, -3);
		add_parti_data->gravity_mass.x = lua_tonumber(p_lua_st, -2);
		add_parti_data->gravity_mass.y = lua_tonumber(p_lua_st, -1);

		ParticleController::Instance()->add_Particle(add_parti_data);

		lua_pop(p_lua_st, map_particle_attri_size);

		lua_pop(p_lua_st, 1);
	}
}
void Map::sort_Map_On_Index()
{
	std::sort(map_piece_list.begin(), map_piece_list.end(), sort_map_index);
}
void Map::set_In_Map_Particle()
{
	in_map_particle_spr = cocos2d::CCSprite::create();
	trigger_draw_node = CCDrawNode::create();

	unsigned int map_total_width = abs(getMapEndPoint().x - getMapStartPoint().x);
	unsigned int map_total_height = cocos2d::CCDirector::sharedDirector()->getWinSize().height;

	in_map_particle_spr->setPosition(cocos2d::CCPoint(getMapStartPoint().x, 0.f));
	in_map_particle_spr->setAnchorPoint(cocos2d::CCPoint(0.f, 0.f));
	in_map_particle_spr->setContentSize(cocos2d::CCSize(map_total_width, map_total_height));

	unsigned int particle_cnt = ParticleController::Instance()->get_Particle_Cnt();
	if (particle_cnt > 0)
	{
		for (unsigned int i = 0; i < particle_cnt; ++i)
		{
			auto iter = ParticleController::Instance()->get_Particle(i);

			in_map_particle_spr->addChild(iter);
		}
	}
	trigger_draw_node->setZOrder(-400);

	ObjectController::Instance()->addChild(in_map_particle_spr);
	ObjectController::Instance()->addChild(trigger_draw_node);
}
void Map::set_In_Map_Object_Resource_Info()
{
	lua_State* p_lua_st = LuaCommunicator::Instance()->Lua_GetState();

	// stage_type
	lua_pushinteger(p_lua_st, 1);
	lua_gettable(p_lua_st, 1);

	lua_getfield(p_lua_st, 1, "stage_type");
	const char* ret_stage_type = lua_tostring(p_lua_st, -1);
	StageManager::Instance()->setStageType(ret_stage_type);

	lua_pop(p_lua_st, 1);

	// in_map_object_sprite files.
	unsigned int in_map_res_list_size = LuaCommunicator::Instance()->getTableSize(2);

	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();

	for(unsigned int i = 1; i <= in_map_res_list_size; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 2);

		const char* ret_name = lua_tostring(p_lua_st, -1);
		cache->addSpriteFramesWithFile(ret_name);

		lua_pop(p_lua_st, 1);
	}

	lua_pop(p_lua_st, 1);
}
void Map::set_Map(map_piece_info& map_piece_data_info)
{
	map_piece* piece_of_map = new map_piece;

	piece_of_map->map_index = map_piece_data_info.map_index;
	piece_of_map->background_image = cocos2d::CCSprite::create(map_piece_data_info.background_image_name);
	piece_of_map->moving_area = map_piece_data_info.moving_area;

	piece_of_map->background_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	cocos2d::CCSize background_img_size = piece_of_map->background_image->getContentSize();
	float map_X = 0.f, map_Y = 0.f;

	if (!map_piece_list.empty())
	{
		float prev_piece_X = 0.f, prev_piece_img_width = 0.f;
		unsigned int map_piece_cnt = map_piece_list.size();
		for (unsigned int i = 0; i < map_piece_cnt; ++i)
		{
			map_piece* prev_piece_iter = map_piece_list.at(i);

			if (prev_piece_iter->map_index == piece_of_map->map_index - 1)
			{
				prev_piece_X = prev_piece_iter->background_image->getPositionX();
				prev_piece_img_width = prev_piece_iter->background_image->getContentSize().width;
				break;
			}
		}
		map_X = (prev_piece_X + prev_piece_img_width);
	}
	else
		map_X = map_piece_data_info.map_index * background_img_size.width;

	cocos2d::CCPoint image_pt = cocos2d::CCPoint(map_X, map_Y);
	piece_of_map->background_image->setPosition(image_pt);

	ObjectController::Instance()->addChild(piece_of_map->background_image);

	piece_of_map->background_image->setZOrder(-500);
	piece_of_map->background_image->retain();

	trigger_list = map_piece_data_info.trigger_list;

	unsigned int in_object_size = map_piece_data_info.in_map_info_list.size();
	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();

	BD_CCLog("%d", in_object_size);
	for(unsigned int i = 0; i < in_object_size; ++i)
	{
		in_map_info* in_map_obj_info_iter = map_piece_data_info.in_map_info_list.at(i);

		in_map_obj* in_map_obj_data = new in_map_obj;
		float in_obj_X = map_X + in_map_obj_info_iter->obj_pos.x , in_obj_Y = map_Y + in_map_obj_info_iter->obj_pos.y;
		GraphicsController::Instance()->getSprite(in_map_obj_data->obj_spr, cache->spriteFrameByName(in_map_obj_info_iter->ref_key));
		in_map_obj_data->obj_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
		in_map_obj_data->obj_spr->setPosition(cocos2d::CCPoint(in_obj_X, in_obj_Y));
		CCSize spr_size = in_map_obj_data->obj_spr->getContentSize();

		in_map_obj_data->obj_spr->retain();

		in_map_obj_data->collision_area = CCRect(in_obj_X - (spr_size.width / 2), in_obj_Y, spr_size.width, spr_size.height);

		BD_CCLog("%s", in_map_obj_info_iter->ref_key);

		ObjectController::Instance()->addChild(in_map_obj_data->obj_spr);
		piece_of_map->in_map_obj_list.push_back(in_map_obj_data);
	}

	map_piece_list.push_back(piece_of_map);
}
void Map::reset_MapCoordinate()
{
	CCPoint start_pt = getMapStartPoint();
	CCPoint end_pt = getMapEndPoint();
	CCSize win_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	float dist = 0.0f;
	if (start_pt.x > 0.0f)
		dist = -start_pt.x;
	else if (end_pt.x < win_size.width)
		dist = win_size.width - end_pt.x;

	setUpdateScrolling(dist);
}
cocos2d::CCPoint Map::getMapStartPoint()
{
	cocos2d::CCPoint first_map_pos = map_piece_list.at(0)->background_image->getPosition();
	cocos2d::CCPoint start_pt = cocos2d::CCPoint(first_map_pos.x, first_map_pos.y);
	return start_pt;
}
cocos2d::CCPoint Map::getMapEndPoint()
{
	int end_index = map_piece_list.size() - 1;
	cocos2d::CCSize background_size = map_piece_list.at(end_index)->background_image->getContentSize();
	cocos2d::CCPoint first_map_pos = map_piece_list.at(end_index)->background_image->getPosition();
	cocos2d::CCPoint end_pt = cocos2d::CCPoint(first_map_pos.x + background_size.width, first_map_pos.y);
	return end_pt;
}
void Map::setUpdateScrolling(int delta_x)
{
	scrolling_val = delta_x;

	trigger_draw_node->clear();

	auto trig_begin = trigger_list.begin();
	auto trig_end = trigger_list.end();

	if (trigger_list.empty())
	{
		trigger_draw_node->setVisible(false);
		trigger_draw_node->setContentSize(cocos2d::CCSize(0.f, 0.f));
	}
	else
	{
		trigger_draw_node->setVisible(true);
		for (auto j = trig_begin; j != trig_end; ++j)
		{
			auto trig_iter = j->second;

			CCPoint trig_iter_pos = trig_iter->trigger_pos;
			CCSize trig_iter_size = trig_iter->trigger_rect.size;

			trig_iter->trigger_rect.setRect(trig_iter_pos.x, trig_iter_pos.y, trig_iter_size.width, trig_iter_size.height);
			if (!strcmp(j->second->trigger_type, "stage_clear"))
			{
				CCPoint draw_rect_pos[4] =
				{
					CCPoint(trig_iter_pos.x, trig_iter_pos.y),
					CCPoint(trig_iter_pos.x + trig_iter_size.width, trig_iter_pos.y),
					CCPoint(trig_iter_pos.x + trig_iter_size.width, trig_iter_pos.y + trig_iter_size.height),
					CCPoint(trig_iter_pos.x, trig_iter_pos.y + trig_iter_size.height)
				};

				ccColor4F fill_color = ccc4f(0.67f, 1.f, 0.18f, 0.3f);
				ccColor4F border_color = ccc4f(0.5f, 1.f, 0.83f, 0.5f);

				trigger_draw_node->drawPolygon(draw_rect_pos, 4, fill_color, 1, border_color);
			}
		}
	}
}