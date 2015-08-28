#pragma once
#include "ObjectProtocol.h"
#include "GameGraphicComponent.h"
class GameGraphicController
{
public:
	GameGraphicController(void);
	~GameGraphicController(void);
public:
	void init();
	void add_Object(obj_info& game_obj_info);
	void update_Object();
	void clear_Object();

	void setAlphaValue(unsigned char val);
	unsigned char getAlphaValue();

	cocos2d::CCRect get_Touched_Area(cocos2d::CCPoint& touched_pt);
	cocos2d::CCRect get_Active_Area();

	void setObjectFlipX(bool isFlip) { isGraphicFlip = isFlip; }
	void setActivateComponent(const char* key) { active_component_key = key; }
	void setRecognizeArea(cocos2d::CCRect recog_area) { this->recog_area = recog_area; }
	void setPosition(cocos2d::CCPoint pos) { draw_pos = pos; }
	void setTargetPosition(cocos2d::CCPoint tar_pos) { target_pos = tar_pos; }
	void send_Message(const char* target_str)
	{
		auto begin = game_graphic_list.begin();
		auto end = game_graphic_list.end();
		auto i = game_graphic_list.begin();

		for(i = begin; i != end; ++i)
		{
			if(!strcmp(target_str, i->first))
				i->second->SetPacket(request_packet);
		}
	}

	cocos2d::CCSprite* getActiveSprite()
	{
		cocos2d::CCSprite* ret_spr = nullptr;

		auto begin = game_graphic_list.begin();
		auto end = game_graphic_list.end();

		for (auto i = begin; i != end; ++i)
		{
			if (!strcmp(i->first, active_component_key))
			{
				ret_spr = i->second->getSprite();
				break;
			}
		}

		return ret_spr;
	}
	const char* getActiveGraphicComp() { return active_component_key; }
	grap_to_obj_packet& get_Message() { return request_packet; }
private:
	grap_to_obj_packet request_packet;
	std::map<const char*, GameGraphicComponent*>game_graphic_list;
	cocos2d::CCDrawNode* recog_area_draw_nd;
	const char* active_component_key;
	cocos2d::CCPoint draw_pos, target_pos;
	cocos2d::CCRect recog_area;
	bool isGraphicFlip;
};

