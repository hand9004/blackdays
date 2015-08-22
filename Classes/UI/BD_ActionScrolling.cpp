#include "BD_ActionScrolling.h"
#include "../Utility/Utility.h"
#include "UILayer.h"

const float restore_val = 0.5f;

BD_ActionScrolling::BD_ActionScrolling(void) : current_scroll_speed(0.0f, 0.0f), scroll_delta(0.0f, 0.0f)
{
}


BD_ActionScrolling::~BD_ActionScrolling(void)
{
}

void BD_ActionScrolling::Init(cocos2d::CCPoint* src_pt, void* action_data)
{
	layer_pt = src_pt;

	layer_act_scroll* scroll_action_data = (layer_act_scroll*)action_data;

	scroll_speed = scroll_action_data->scrollSpeed;
	scroll_mode = scroll_action_data->scroll_mode;
}
void BD_ActionScrolling::Update()
{
	// 제한된 속도를 넘었을 경우, 제한된 속도로 고정시키고 스크롤링을 시작한다.
	if(scroll_speed < scroll_delta.x)
		scroll_delta.x = scroll_speed;
	else if(-scroll_speed > scroll_delta.x)
		scroll_delta.x = -scroll_speed;

	if(scroll_speed < scroll_delta.y)
		scroll_delta.y = scroll_speed;
	else if(-scroll_speed > scroll_delta.y)
		scroll_delta.y = -scroll_speed;

	layer_pt->x += scroll_delta.x;
	layer_pt->y += scroll_delta.y;

	// 스크롤링 변수를 천천히 줄이면서 부드러운 스크롤 효과를 내준다.
	if(scroll_delta.y > restore_val)
		scroll_delta.y -= restore_val;
	else if (scroll_delta.y < -restore_val)
		scroll_delta.y += restore_val;
	else if(scroll_delta.y <= restore_val && scroll_delta.y >= -restore_val)
		scroll_delta.y = 0.0f;

	if (scroll_delta.x > restore_val)
		scroll_delta.x -= restore_val;
	else if (scroll_delta.x < -restore_val)
		scroll_delta.x += restore_val;
	else if (scroll_delta.x <= restore_val && scroll_delta.x >= -restore_val)
		scroll_delta.x = 0.0f;
	
	BD_CCLog("scroll_delta = %f %f", scroll_delta.x, scroll_delta.y);

	vertical_pos_reset();
	horizontal_pos_reset();
}
void BD_ActionScrolling::Destroy()
{
	scroll_mode = nullptr;
}
void BD_ActionScrolling::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt)
{
	switch(t_event)
	{
	case TOUCH_BEGIN:
		scroll_delta = cocos2d::CCPoint(0.0f, 0.0f);
		break;
	case TOUCH_MOVED:
		scroll_delta = delta_pt;
		break;
	case TOUCH_ENDED:
		break;
	}
	if(!strcmp(scroll_mode, "vertical"))
		scroll_delta.x = 0.0f;
	else if(!strcmp(scroll_mode, "horizontal"))
		scroll_delta.y = 0.0f;
}
bool BD_ActionScrolling::vertical_pos_reset()
{
	bool v_chk = false;
	cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	float layer_top = layer_pt->y, layer_bottom = (layer_pt->y + src_layer_size.height);

	if(src_layer_size.height > win_size.height)
	{
		if(layer_top <= 0 && layer_bottom >= win_size.height)
			v_chk = true;
		else
		{
			v_chk = false;
			
			if(layer_top > 0)
				layer_pt->y += -layer_pt->y;
			else if(layer_bottom < win_size.height)
				layer_pt->y += (win_size.height - layer_bottom);

			scroll_delta.y = 0.0f;
		}
	}
	else
	{
		if(layer_top >= 0 && layer_bottom <= win_size.height)
			v_chk = true;
		else
		{
			v_chk = false;

			if(layer_top < 0)
				layer_pt->y += -layer_pt->y;
			else if(layer_bottom > win_size.height)
				layer_pt->y += (win_size.height - layer_bottom);

			scroll_delta.y = 0.0f;
		}
	}

	return v_chk;
}
bool BD_ActionScrolling::horizontal_pos_reset()
{
	bool h_chk = false;
	cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	float layer_left = layer_pt->x, layer_right = (layer_pt->x + src_layer_size.width);
		
	// 스크롤하고자 하는 레이어의 크기와 윈도우즈의 크기를 비교하여, 처리를 다르게 하는 로직.
	if(src_layer_size.width > win_size.width)
	{
		if(layer_left <= 0 && layer_right >= win_size.width)
			h_chk = true;
		else
		{
			h_chk = false;

			if(layer_left > 0)
				layer_pt->x += -layer_pt->x;
			else if(layer_right < win_size.width)
				layer_pt->x += (win_size.width - layer_right);

			scroll_delta.x = 0.0f;
		}
	}
	else
	{
		if(layer_left >= 0 && layer_right <= win_size.width)
			h_chk = true;
		else
		{
			h_chk = false;

			if(layer_left < 0)
				layer_pt->x += -layer_pt->x;
			else if(layer_right > win_size.width)
				layer_pt->x += (win_size.width - layer_right);

			scroll_delta.x = 0.0f;
		}
	}

	return h_chk;
}