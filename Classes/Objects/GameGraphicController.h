#pragma once
#include "ObjectProtocol.h"
#include "GameGraphicComponent.h"
class GameGraphicController
{
public:
	GameGraphicController(void);
	~GameGraphicController(void);
public:
	void add_Object(obj_info& game_obj_info);
	void update_Object();
	void clear_Object();

	void setAlphaValue(unsigned char val);
	unsigned char getAlphaValue();

	cocos2d::CCRect get_Touched_Area(cocos2d::CCPoint& touched_pt);
	cocos2d::CCRect get_Active_Area();

	void setObjectFlipX(bool isFlip) { isGraphicFlip = isFlip; }
	void setActivateComponent(const char* key) { active_component_key = key; }
	void setPosition(cocos2d::CCPoint pos) { draw_pos = pos; }
	void setTargetPosition(cocos2d::CCPoint tar_pos) { target_pos = tar_pos; }
	void send_Message(const char* target_str)
	{
		auto begin = game_graphic_list.begin();
		auto end = game_graphic_list.end();
		auto i = game_graphic_list.begin();

		for(i = game_graphic_list.begin(); i != end; ++i)
		{
			if(!strcmp(target_str, i->first))
				i->second->SetPacket(request_packet);
		}
	}

	const char* getActiveGraphicComp() { return active_component_key; }
	grap_to_obj_packet& get_Message() { return request_packet; }
private:
	grap_to_obj_packet request_packet;
	std::map<const char*, GameGraphicComponent*>game_graphic_list;
	const char* active_component_key;
	cocos2d::CCPoint draw_pos, target_pos;
	bool isGraphicFlip;
};

