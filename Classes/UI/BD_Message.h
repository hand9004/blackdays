#pragma once
#include "UIComponent.h"
class BD_Message : public UIComponent
{
public:
	BD_Message(void);
	~BD_Message(void);
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
	char message[512];
	cocos2d::CCLabelTTF* message_label;
	cocos2d::CCSprite* background_image;
};

