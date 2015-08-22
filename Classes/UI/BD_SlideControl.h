#pragma once
#include "UIComponent.h"
class BD_SlideControl : public UIComponent
{
public:
	BD_SlideControl(void);
	~BD_SlideControl(void);
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
	unsigned int currentSlidedValue; // Range 0 ~ 100
	cocos2d::CCPoint slide_bar_pos, slide_bar_real_pos;
	cocos2d::CCSprite* background_image, *slide_bar_image;
	cocos2d::CCLabelBMFont* test_font;
	int bar_x_mid_val;
};