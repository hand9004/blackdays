#include "BD_Button.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"
#include <SimpleAudioEngine.h>

BD_Button::BD_Button(void) : button_image(nullptr), pressed_button_image(nullptr), isClicked(false)
{
}

BD_Button::~BD_Button(void)
{
}

void BD_Button::Init(void* packet)
{
	setResource(packet);
}

bool BD_Button::get_UIActive()
{
	return (button_image->isVisible() || pressed_button_image->isVisible());
}

void BD_Button::set_UIActive(bool isActive)
{
	if(!isActive)
	{
		button_image->setVisible(false);
		pressed_button_image->setVisible(false);
	}
	else
	{
		button_image->setVisible(true);
		pressed_button_image->setVisible(false);
	}
}
void BD_Button::Update()
{
}
void BD_Button::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
{
	set_UIStatus(t_event);

	bool is_touch_enable = this->isTouchEnabled();
	if(is_touch_enable)
	{
		switch(t_event)
		{
		case TOUCH_BEGIN:
			SoundManager::Instance()->play_EffectMusic("sounds/button_on_click.mp3");
			button_image->setVisible(false);
			pressed_button_image->setVisible(true);
			break;
		case TOUCH_MOVED:
		case TOUCH_ENDED:
			isClicked = true;
			button_image->setVisible(true);
			pressed_button_image->setVisible(false);
			break;
		}
	}
	else
	{
		isClicked = false;
		button_image->setVisible(true);
		pressed_button_image->setVisible(false);
	}
}

void BD_Button::Destroy()
{
	CCAssert(button_image != nullptr, "BD_Button : ButtonImage Already nullptr");
	CCAssert(pressed_button_image != nullptr, "BD_Button : PressedButtonImage Already nullptr");

	removeChild(button_image, true);
	removeChild(pressed_button_image, true);
}
void* BD_Button::send_message_main()
{
	return (void*)isClicked;
}
void* BD_Button::send_message_lua()
{
	return nullptr;
}
void BD_Button::recv_message_main(void* src_msg)
{
	isClicked = static_cast<bool>(src_msg);
}

void BD_Button::recv_message_lua(void* src_msg)
{
}

void BD_Button::setResource(void* packet)
{
	butt_pack* butt_packet = static_cast<butt_pack*>(packet);

	setPosition(butt_packet->image_info.image_pt);

	cocos2d::CCPoint button_pos = cocos2d::CCPoint(0.0f, 0.0f);
	cocos2d::CCPoint pressed_pos = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(button_image, butt_packet->image_info.file_name, &button_pos);
	GraphicsController::Instance()->getSprite(pressed_button_image, butt_packet->pressed_image_info.file_name, 
							&pressed_pos);

	button_image->setVisible(true);
	pressed_button_image->setVisible(false);

	button_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	pressed_button_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	this->setContentSize(button_image->getContentSize());

	addChild(button_image);
	addChild(pressed_button_image);

	SAFE_DELETE(butt_packet);
}

