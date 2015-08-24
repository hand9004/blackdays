#pragma once
#include <cocos2d.h>
#include "UI_Def.h"
#include "../SoundManager.h"

USING_NS_CC;

const float swipe_disable_range = 5.0f;

class UIComponent : public cocos2d::CCLayer
{
public:
	UIComponent(void);
	virtual ~UIComponent(void);
public:
	virtual void Init(void* packet) = 0;
	virtual bool get_UIActive() = 0;
	virtual void set_UIActive(bool isActive) = 0;
	virtual void Update() = 0;
	virtual void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt) = 0;
	virtual void Destroy() = 0;
public:
	// UI 객체가 메인 로직으로 정보를 보내기 위한 함수
	virtual void* send_message_main() = 0;
	// UI 객체가 루아로 정보를 보내기 위한 함수
	virtual void* send_message_lua() = 0;

	// UI 객체가 메인 로직으로부터 정보를 받기 위한 함수
	virtual void recv_message_main(void* src_msg) = 0;
	// UI 객체가 루아로부터 정보를 받기 위한 함수
	virtual void recv_message_lua(void* src_msg) = 0;
public:
	void set_ID(unsigned int id) { this->id |= id; }
	void set_Sort(unsigned int sort) { id &= ~UI_TYPE_FIELD; id |= (sort << UI_TYPE_SHIFT); }
	void set_Message(unsigned int message) { id &= ~UI_MESSAGE_FIELD; id |= (message << UI_MESSAGE_SHIFT); }
	void set_UIStatus(unsigned int ui_stat) { id &= ~UI_TOUCH_EVENT_FIELD; id |= (ui_stat << UI_TOUCH_EVENT_SHIFT); }

	unsigned int get_ID() { return id; }
	unsigned int get_PureID() { return (id & (UI_TYPE_FIELD | UI_ID_FIELD)); }
	unsigned int get_Sort() { return (id & UI_TYPE_FIELD) >> UI_TYPE_SHIFT; }
	unsigned int get_Message() { return (id & UI_MESSAGE_FIELD) >> UI_MESSAGE_SHIFT; }
	unsigned int get_UIStatus() { return (id & UI_TOUCH_EVENT_FIELD) >> UI_TOUCH_EVENT_SHIFT; }
public:
	void setIsSwipeMode(bool isSwipe) { isSwipeMode = isSwipe; }
	bool getIsSwipeMode() { return isSwipeMode; }
	bool is_UITouched(cocos2d::CCPoint& touched_Pt)
	{
		bool ui_active = get_UIActive();
		cocos2d::CCPoint ui_pos = getPosition();
		cocos2d::CCSize contents_size = getContentSize();
		if(ui_active)
		{
			cocos2d::CCRect contents_Rect = cocos2d::CCRect(ui_pos.x, ui_pos.y, contents_size.width, contents_size.height);

			if(contents_Rect.containsPoint(touched_Pt))
				return true;
			else
				return false;
		}
		else
			return false;
	}
protected:
	virtual void setResource(void* packet) = 0;
protected:
	cocos2d::CCRect ui_rect;
	unsigned int id; // UI 식별자 및 UI별 메시지 포함.(enum으로 정의 예정)
	// 부호 없는 int자료형의 4byte중 LSB로부터 12bit는 ID 값이며, 다음 4bit는 UI 종류 비트이며,
	// 이후 4bit는 현재 UI의 상태를 나타내며, 그 이후 12bit는 UI가 보내는 메시지에 해당한다.
	// UI의 메시지는 Check_Box일 경우, 체크된 상태, Slide_Control의 경우 현재 바의 위치에 해당된다.
	void* recv_message_by_main;
	bool isUISelected, isSwipeMode;
};