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
	void Update_ScrollingPos_HUD(float delta_X);
	void Destroy_HUD();
public:
	void setOnRun(bool onRun) { this->onRun = onRun; }
	void setOnHide(bool onHide) { this->onHide = onHide; }
	void setCurrentEvent(obj_event curr_ev) { this->curr_ev = curr_ev; }
	void setObjectImageSize(cocos2d::CCSize src_size) { current_image_size = src_size; }
	void setMovePoint(cocos2d::CCPoint move_point) { this->move_point = move_point; }
	void setTapCount(unsigned int tap_count) { this->tap_count = tap_count; }
	void setMaxTapCount(unsigned int max_tap_count) { this->max_tap_count = max_tap_count; }

	void isSelected(bool isSelected) { isSelectObject = isSelected; }
	void isAttacked(bool isAttacked) { isAttackedByEnemy = isAttacked; }
private:
	void gauge_update();
	void selected_update();
	void status_update();
private:
	obj_event curr_ev;
	obj_info* game_obj_info;
	bool isSelectObject, isAttackedByEnemy, onRun, onHide;
	cocos2d::CCSize current_image_size;
	cocos2d::CCSprite* hp_frame, *hp_gauge, *cc_frame, *cc_gauge, *current_select, *move_select, *hide_frame, *run_frame, *sleep_frame, *tap_image;
	cocos2d::CCPoint move_point;
	float tap_count, max_tap_count;
	long elapsed_sleep_time, sleep_duration;
};

