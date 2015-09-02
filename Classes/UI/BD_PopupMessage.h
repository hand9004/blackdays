#pragma once
#include "UIComponent.h"
class BD_PopupMessage : public UIComponent
{
public:
	BD_PopupMessage(void);
	~BD_PopupMessage(void);
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
	cocos2d::CCLabelBMFont* pop_font_label;
	char message_mode[16], message[512];
	unsigned long delay_time_ms, start_time, end_time;
	unsigned char opacity_val;
	bool onEvent, onReset;
};

