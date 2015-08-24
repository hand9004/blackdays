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
	// UI ��ü�� ���� �������� ������ ������ ���� �Լ�
	virtual void* send_message_main() = 0;
	// UI ��ü�� ��Ʒ� ������ ������ ���� �Լ�
	virtual void* send_message_lua() = 0;

	// UI ��ü�� ���� �������κ��� ������ �ޱ� ���� �Լ�
	virtual void recv_message_main(void* src_msg) = 0;
	// UI ��ü�� ��Ʒκ��� ������ �ޱ� ���� �Լ�
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
	unsigned int id; // UI �ĺ��� �� UI�� �޽��� ����.(enum���� ���� ����)
	// ��ȣ ���� int�ڷ����� 4byte�� LSB�κ��� 12bit�� ID ���̸�, ���� 4bit�� UI ���� ��Ʈ�̸�,
	// ���� 4bit�� ���� UI�� ���¸� ��Ÿ����, �� ���� 12bit�� UI�� ������ �޽����� �ش��Ѵ�.
	// UI�� �޽����� Check_Box�� ���, üũ�� ����, Slide_Control�� ��� ���� ���� ��ġ�� �ش�ȴ�.
	void* recv_message_by_main;
	bool isUISelected, isSwipeMode;
};