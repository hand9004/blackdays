#pragma once
#include "UIComponent.h"
class BD_CheckBox : public UIComponent
{
public:
	BD_CheckBox(void);
	~BD_CheckBox(void);
public:
	virtual void Init(void* packet);
	virtual bool get_UIActive();
	virtual void set_UIActive(bool isActive);
	virtual void Update();
	virtual void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt);
	virtual void Destroy();
public:
	virtual void* send_message_main();
	virtual void* send_message_lua();

	virtual void recv_message_main(void* src_msg);
	virtual void recv_message_lua(void* src_msg);
private:
	virtual void setResource(void* packet);
private:
	bool isChecked;
	cocos2d::CCSprite* checked_image, *unchecked_image;
};

