#include "BD_PopupMessage.h"
#include "../Utility/Utility.h"
#include "UIController.h"

BD_PopupMessage::BD_PopupMessage(void) : start_time(0), end_time(0), opacity_val(0), onEvent(false), onReset(false)
{
}


BD_PopupMessage::~BD_PopupMessage(void)
{
}

void BD_PopupMessage::Init(void* packet)
{
	setResource(packet);
	start_time = get_ms_onSystem();
}
bool BD_PopupMessage::get_UIActive()
{
	return this->isVisible();
}
void BD_PopupMessage::set_UIActive(bool isActive)
{
	this->setVisible(isActive);
}
void BD_PopupMessage::Update()
{
	if(!strcmp(message_mode, "toast"))
	{
		bool isUIActive = get_UIActive();
		if(isUIActive)
		{
			if(onEvent && !onReset)
			{
				start_time = get_ms_onSystem();
				onReset = true;
			}

			end_time = get_ms_onSystem();
			unsigned long delta_time = end_time - start_time;

			if(delta_time >= delay_time_ms)
			{
				set_UIActive(false);
				end_time = 0;
				start_time = 0;
				onReset = false;
			}
			else
			{
				float time_percentage = ((float)delta_time / (float)delay_time_ms);
				const unsigned long triple_div_time = delay_time_ms / 3;

				if(delta_time < triple_div_time)
				{
					float increase_val = (time_percentage * 3);
					opacity_val = (unsigned char)(increase_val * 255);
				}
				else if(delta_time >= triple_div_time && delta_time < triple_div_time * 2)
					opacity_val = 255;
				else
				{
					float decrease_val = 1 - time_percentage;
					opacity_val = (unsigned char)((decrease_val * 255) * 3);
				}
//				BD_CCLog("delta_time = %d Opacity_Val = %d", delta_time, opacity_val);
				pop_font_label->setOpacity(opacity_val);
			}
		}
	}
	else if(!strcmp(message_mode, "normal"))
	{
	}
}
void BD_PopupMessage::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
{
	set_UIStatus(t_event);

	bool is_touch_enable = this->isTouchEnabled();
	if(is_touch_enable)
	{
		switch(t_event)
		{
		case TOUCH_BEGIN:
			break;
		case TOUCH_MOVED:
			break;
		case TOUCH_ENDED:
			break;
		}
	}
}
void BD_PopupMessage::Destroy()
{
	CCAssert(pop_font_label != nullptr, "BD_PopupMessage : PopupMessage Label Already nullptr");
	
	removeChild(pop_font_label, true);
}
void* BD_PopupMessage::send_message_main()
{
	return nullptr;
}
void* BD_PopupMessage::send_message_lua()
{
	return nullptr;
}
void BD_PopupMessage::recv_message_main(void* src_msg)
{
	bool onEventUI = static_cast<bool>(src_msg);

	onEvent = onEventUI;
}
void BD_PopupMessage::recv_message_lua(void* src_msg)
{
	pop_msg_update_pack* pop_mess_update_pack = static_cast<pop_msg_update_pack*>(src_msg);

	pop_font_label->setString(pop_mess_update_pack->message);

	start_time = get_ms_onSystem();

	SAFE_DELETE(pop_mess_update_pack);
}
void BD_PopupMessage::setResource(void* packet)
{
	char font_full_path[512] = {0,};

	pop_message_pack* pop_mess_pack = static_cast<pop_message_pack*>(packet);

	setPosition(pop_mess_pack->message_pt);
	delay_time_ms = pop_mess_pack->delay_time_ms;

	strncpy(message_mode, pop_mess_pack->message_mode, sizeof(message_mode));
	strncpy(message, pop_mess_pack->message, sizeof(message));

//	strcpy(font_full_path, "fonts/");
	strncat(font_full_path, pop_mess_pack->font_name, sizeof(font_full_path));

	pop_font_label = cocos2d::CCLabelTTF::create();
	pop_font_label->initWithString(message, font_full_path, pop_mess_pack->font_size);
//	pop_font_label->setString(message);
//	pop_font_label->setFontName(font_full_path);
//	pop_font_label->setFontSize(90);
	pop_font_label->setColor(pop_mess_pack->color_val);
	pop_font_label->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
	pop_font_label->setOpacity(0);

	cocos2d::CCSize font_size = pop_font_label->getContentSize();

	this->setContentSize(pop_font_label->getContentSize());

	addChild(pop_font_label);

	SAFE_DELETE(pop_mess_pack);
}