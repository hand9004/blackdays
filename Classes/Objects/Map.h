#pragma once
#include <cocos2d.h>

typedef enum MAP_OBJECT_TYPE
{
	OBSTACLE_OBJECT,
	HIDE_OBJECT,
	HELPER_OBJECT,
}map_obj_type;

typedef struct MAP_TRIGGER_INFO
{
	cocos2d::CCPoint trigger_pos;
	cocos2d::CCRect trigger_rect;
}map_trig_info;

typedef struct IN_MAP_INFO
{
	cocos2d::CCPoint obj_pos;
	const char* ref_list, *ref_key;
	map_obj_type in_map_obj_type;
}in_map_info;

typedef struct MAP_INFO
{
	int map_index;
	cocos2d::CCRect moving_area;
	const char* background_image_name;
	std::vector<in_map_info*>in_map_info_list;
	std::map<const char*, map_trig_info*>trigger_list;
}map_piece_info;

typedef struct IN_MAP_OBJECT
{
	cocos2d::CCSprite* obj_spr;
	map_obj_type in_map_obj_type;
	cocos2d::CCRect collision_area;
}in_map_obj;

typedef struct MAP_PIECE
{
	int map_index;
	cocos2d::CCRect moving_area;
	cocos2d::CCSprite* background_image;
	std::vector<in_map_obj*>in_map_obj_list;
}map_piece;

class Map
{
	Map(void);
public:
	~Map(void);

	static Map* Instance()
	{
		static Map* map_inst = nullptr;
		
		if(map_inst == nullptr)
			map_inst = new Map;

		return map_inst;
	}
public:
	void Init();
	void Update();
	void Destroy();

	void register_MapFunction();
public:
	void setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	bool isInMovingArea(cocos2d::CCPoint src_pt);

	const char* get_collided_trigger(cocos2d::CCPoint src_pt);
	void reset_MapCoordinate();

	bool getIsCanScrolling();
	cocos2d::CCPoint getMapStartPoint();
	cocos2d::CCPoint getMapEndPoint();

	void setUpdateScrolling(float delta_x);
	std::vector<map_piece*>getMapPieceList() { return map_piece_list; }
public:
	void setLuaState(lua_State* lua_st) { p_lua_st = lua_st; }
private:
	void set_Map_Info();
	void sort_Map_On_Index();
	void set_In_Map_Particle();
	void set_In_Map_Object_Resource_Info();
	void set_Map(map_piece_info& map_piece_data_info);
private:
	cocos2d::CCDrawNode* trigger_draw_node;
	cocos2d::CCSprite* in_map_particle_spr;
	std::vector<map_piece*>map_piece_list;
	std::map<const char*, map_trig_info*>trigger_list;
	lua_State* p_lua_st;
};

