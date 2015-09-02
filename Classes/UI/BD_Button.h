#pragma once
#include "UIComponent.h"
class BD_Button : public UIComponent
{
public:
	BD_Button(void);
	~BD_Button(void);
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
public:
	void setButtonImage(cocos2d::CCTexture2D* butt_img, cocos2d::CCTexture2D* pressed_butt_img);
private:
	virtual void setResource(void* packet);
private:
	cocos2d::CCSprite* button_image, *pressed_button_image;
	bool isClicked;
};