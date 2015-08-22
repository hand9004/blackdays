#pragma once
#include "UIComponent.h"
class BD_SlideSelector : public UIComponent
{
public:
	BD_SlideSelector(void);
	~BD_SlideSelector(void);
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
	cocos2d::CCSprite* background_image, *talk_balloon_image;
	cocos2d::CCPoint touch_begin_pt, latest_slided_dir_vec;
	unsigned int current_index, list_size;
	bool isVertical;
};

