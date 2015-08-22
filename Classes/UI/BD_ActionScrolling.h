#pragma once
#include "BD_LayerAction.h"
class BD_ActionScrolling : public BD_LayerAction
{
public:
	BD_ActionScrolling(void);
	~BD_ActionScrolling(void);
public:
	virtual void Init(cocos2d::CCPoint* src_pt, void* action_data);
	virtual void Update();
	virtual void Destroy();
public:
	virtual void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt);
private:
	bool vertical_pos_reset();
	bool horizontal_pos_reset();
private:
	const char* scroll_mode;
	float scroll_speed;
	cocos2d::CCPoint current_scroll_speed, scroll_delta, *layer_pt;
};
