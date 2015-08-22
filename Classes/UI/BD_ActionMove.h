#pragma once
#include "BD_LayerAction.h"
class BD_ActionMove : public BD_LayerAction
{
public:
	BD_ActionMove(void);
	~BD_ActionMove(void);
public:
	virtual void Init(cocos2d::CCPoint* src_pt, void* action_data);
	virtual void Update();
	virtual void Destroy();
public:
	virtual void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt);
private:
	cocos2d::CCPoint dest_pt, start_pt;
	cocos2d::CCPoint* current_pt;
	float moveSpeed;
	bool isVerticalMove;
};

