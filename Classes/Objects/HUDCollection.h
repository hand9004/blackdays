#pragma once
#include <cocos2d.h>
#include "ObjectProtocol.h"

class HUDCollection
{
public:
	HUDCollection(void);
	~HUDCollection(void);
public:
	void Init_HUD(obj_info& game_object_info);
	void Update_HUD();
	void Destroy_HUD();
public:
	void isSelected(bool isSelected) { isSelectObject = isSelected; }
	void setObjectImageSize(cocos2d::CCSize src_size) { current_image_size = src_size; }
	void isAttacked(bool isAttacked) { isAttackedByEnemy = isAttacked; }
private:
	void hp_update();
	void selected_update();
	void attacked_update();
private:
	obj_info* game_obj_info;
	bool isSelectObject, isAttackedByEnemy;
	cocos2d::CCSize current_image_size;
	cocos2d::CCSprite* hp_frame, *hp_gauge, *current_select, *attacked_select;
};

