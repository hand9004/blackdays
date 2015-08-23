#include "Map.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"
#include "ParticleController.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

USING_NS_CC;

#include <algorithm>

int l_add_Map(lua_State* L)
{
	Map::Instance()->setLuaState(L);

	const char *table_name = nullptr;

	table_name = lua_tostring(L, 1);

	lua_pop(L, 1);

	lua_getglobal(L, table_name);
	CCAssert(lua_istable(L, -1), "Error : Wasn't Lua Table Type.");

	Map::Instance()->Init();

	return 0;
}

bool sort_map_index(map_piece* fir, map_piece* sec)
{
	return fir->map_index < sec->map_index;
}

Map::Map(void) : in_map_particle_spr(nullptr)
{
}

Map::~Map(void)
{
}

void Map::Init()
{
	in_map_particle_spr = cocos2d::CCSprite::create();

	set_In_Map_Object_Resource_Info();

	set_Map_Info();

	sort_Map_On_Index();

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
	in_map_particle_spr->setZOrder(2);

	ObjectController::Instance()->addChild(in_map_particle_spr);
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
			in_map_obj* in_map_obj_iter = map_piece_iter->in_map_obj_list[j];

			ObjectController::Instance()->removeChild(in_map_obj_iter->obj_spr, true);

			SAFE_DELETE(in_map_obj_iter);
		}
		vector_clear(map_piece_iter->in_map_obj_list);

		ObjectController::Instance()->removeChild(map_piece_iter->background_image, true);

		SAFE_DELETE(map_piece_iter);
	}
	vector_clear(map_piece_list);

	if (in_map_particle_spr != nullptr)
	{
		in_map_particle_spr->removeAllChildrenWithCleanup(true);

		ParticleController::Instance()->destroy_Particle();
	}
}
void Map::register_MapFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("add_map", l_add_Map);
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

	int end_index = map_piece_list.size() - 1;

	bool isCanMove = false;
	unsigned int map_size = map_piece_list.size();
	for(unsigned int i = 0; i < map_size; ++i)
	{
		cocos2d::CCPoint pt = map_piece_list.at(i)->background_image->getPosition();
		float width = map_piece_list.at(i)->moving_area.size.width, height = map_piece_list.at(i)->moving_area.size.height;
		can_move_Rect = cocos2d::CCRect(pt.x, 0.0f, width, height);

		if(can_move_Rect.containsPoint(src_pt))
			isCanMove = true;
	}

	return isCanMove;
}
void Map::set_Map_Info()
{
	const int constant_data_size = 3;
	unsigned int map_piece_size = LuaCommunicator::Instance()->getTableSize(1);

	// 맵의 실제 정보로부터 시작한다. 1번째는 맵에 들어갈 오브젝트들을 레퍼런스할 plist가 들어가 있다.
	for(unsigned int i = 2; i <= map_piece_size - 1; ++i)
	{
		lua_pushinteger(p_lua_st, i);
		lua_gettable(p_lua_st, 1);

		map_piece_info map_piece_data;

		// start Preset Info
		lua_pushstring(p_lua_st, "index");
		lua_gettable(p_lua_st, 2);

		lua_pushstring(p_lua_st, "background");
		lua_gettable(p_lua_st,  2);

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

		set_Map(map_piece_data);

		unsigned int in_obj_size = map_piece_data.in_map_info_list.size();

		for(unsigned int j = 0; j < in_obj_size; ++j)
			SAFE_DELETE(map_piece_data.in_map_info_list[j]);
		
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
void Map::set_In_Map_Object_Resource_Info()
{
	lua_pushinteger(p_lua_st, 1);
	lua_gettable(p_lua_st, 1);

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
	float map_X = map_piece_data_info.map_index * background_img_size.width, map_Y = 0.0f;

	cocos2d::CCPoint image_pt = cocos2d::CCPoint(map_X, map_Y);
	piece_of_map->background_image->setPosition(image_pt);

	ObjectController::Instance()->addChild(piece_of_map->background_image);

	unsigned int in_object_size = map_piece_data_info.in_map_info_list.size();

	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();

	BD_CCLog("%d", in_object_size);
	for(unsigned int i = 0; i < in_object_size; ++i)
	{
		in_map_info* in_map_obj_info_iter = map_piece_data_info.in_map_info_list[i];

		in_map_obj* in_map_obj_data = new in_map_obj;
		float in_obj_X = map_X + in_map_obj_info_iter->obj_pos.x , in_obj_Y = map_Y + in_map_obj_info_iter->obj_pos.y;
		GraphicsController::Instance()->getSprite(in_map_obj_data->obj_spr, cache->spriteFrameByName(in_map_obj_info_iter->ref_key));
		in_map_obj_data->obj_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
		in_map_obj_data->obj_spr->setPosition(cocos2d::CCPoint(in_obj_X, in_obj_Y));
		CCSize spr_size = in_map_obj_data->obj_spr->getContentSize();

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
bool Map::getIsCanScrolling()
{
	cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	if(!map_piece_list.empty())
	{
		bool canLeftScrolling = false;
		bool canRightScrolling = false;

		cocos2d::CCPoint start_pt = getMapStartPoint();
		cocos2d::CCPoint end_pt = getMapEndPoint();

		if(start_pt.x < 0.0f)
			canLeftScrolling = true;
		else
			canLeftScrolling = false;

		if(end_pt.x > win_size.width)
			canRightScrolling = true;
		else
			canRightScrolling = false;

		return (canLeftScrolling && canRightScrolling);
	}
	else
		return false;
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
void Map::setUpdateScrolling(float delta_x)
{
	unsigned int obj_size = map_piece_list.size();
	unsigned int start_map_index = 0, end_map_index = obj_size - 1;

	for (unsigned int i = 0; i < obj_size; ++i)
	{
		map_piece* map_piece_iter = map_piece_list.at(i);
		CCPoint background_pos = map_piece_iter->background_image->getPosition();

		background_pos.x += delta_x;

		unsigned int in_map_obj_size = map_piece_iter->in_map_obj_list.size();

		for (unsigned int j = 0; j < in_map_obj_size; ++j)
		{
			in_map_obj* in_map_obj_iter = map_piece_iter->in_map_obj_list[j];

			CCPoint obj_pos = in_map_obj_iter->obj_spr->getPosition();

			CCSize obj_size = in_map_obj_iter->obj_spr->getContentSize();

			obj_pos.x += delta_x;
			
			in_map_obj_iter->collision_area.setRect(obj_pos.x - (obj_size.width / 2), obj_pos.y, obj_size.width, obj_size.height);

			in_map_obj_iter->obj_spr->setPosition(obj_pos);
		}

		map_piece_iter->background_image->setPosition(background_pos);
	}

	CCPoint particle_pt = in_map_particle_spr->getPosition();
	particle_pt.x += delta_x;
	in_map_particle_spr->setPosition(particle_pt);
}