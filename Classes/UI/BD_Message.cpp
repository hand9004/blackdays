#include "BD_Message.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"


BD_Message::BD_Message(void) : message_label(nullptr), background_image(nullptr)
{
}

BD_Message::~BD_Message(void)
{
}

void BD_Message::Init(void* packet)
{
	setResource(packet);
}

bool BD_Message::get_UIActive()
{
	return (background_image->isVisible() || message_label->isVisible());
}

void BD_Message::set_UIActive(bool isActive)
{
	if(!isActive)
	{
		background_image->setVisible(false);
		message_label->setVisible(false);
	}
	else
	{
		background_image->setVisible(true);
		message_label->setVisible(true);
	}
}
void BD_Message::Update()
{
}
void BD_Message::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
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
			background_image->setVisible(false);
			message_label->setVisible(false);
			break;
		}
	}
}

void BD_Message::Destroy()
{
	CCAssert(background_image != nullptr, "BD_Message : BackgroundImage Already nullptr");
	CCAssert(message_label != nullptr, "BD_Message : MessageLabel Already nullptr");

	removeChild(background_image, true);
	removeChild(message_label, true);
}
void* BD_Message::send_message_main()
{
	return nullptr;
}
void* BD_Message::send_message_lua()
{
	return nullptr;
}
void BD_Message::recv_message_main(void* src_msg)
{
}
void BD_Message::recv_message_lua(void* src_msg)
{
}
void BD_Message::setResource(void* packet)
{
	message_pack* mess_pack = static_cast<message_pack*>(packet);

	setPosition(mess_pack->background_image_info.image_pt);

	strncpy(message, mess_pack->message, sizeof(message));

	cocos2d::CCPoint background_pos = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(background_image, mess_pack->background_image_info.file_name,
						&background_pos);

	background_image->setVisible(true);
	background_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	background_pos = background_image->getPosition();

	message_label = cocos2d::CCLabelTTF::create();
	message_label->setFontSize(30);
	message_label->setString(message);
	message_label->setFontName("fonts/arial.ttf");
	message_label->setColor(cocos2d::ccc3(0, 0, 0));
	message_label->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	cocos2d::CCSize message_size = message_label->getContentSize();

	cocos2d::CCSize contents_size = background_image->getContentSize();
	this->setContentSize(contents_size);

	float ui_mid_X = background_pos.x + (contents_size.width / 2), ui_mid_Y = background_pos.y + (contents_size.height / 2);
	float message_X = ui_mid_X - (message_size.width / 2), message_Y = ui_mid_Y - (message_size.height / 2);
	cocos2d::CCPoint message_pt = cocos2d::CCPoint(message_X, message_Y);
	message_label->setPosition(message_pt);

	addChild(background_image);
	addChild(message_label);

	SAFE_DELETE(mess_pack);
}