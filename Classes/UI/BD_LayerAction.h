#pragma once
#include <cocos2d.h>
#include "UI_Def.h"
#include "UIComponent.h"
class BD_LayerAction
{
public:
	BD_LayerAction(void);
	virtual ~BD_LayerAction(void);
public:
	virtual void Init(cocos2d::CCPoint* src_pt, void* action_data) = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
public:
	virtual void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt) = 0;
public:
	void setLayerSize(cocos2d::CCSize size) { src_layer_size = size; }
	bool getActionEnded() { return isActionEnded; }
protected:
	bool isActionEnded;
	cocos2d::CCSize src_layer_size;
};

