#include "BD_CheckBox.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"

BD_CheckBox::BD_CheckBox(void) : isChecked(false), checked_image(nullptr), unchecked_image(nullptr)
{
}


BD_CheckBox::~BD_CheckBox(void)
{
}

void BD_CheckBox::Init(void* packet)
{
	setResource(packet);
}

bool BD_CheckBox::get_UIActive()
{
	return unchecked_image->isVisible();
}

void BD_CheckBox::set_UIActive(bool isActive)
{
	if(!isActive)
	{
		checked_image->setVisible(false);
		unchecked_image->setVisible(false);
	}
	else
	{
		if(isChecked)
			checked_image->setVisible(true);
		else
			checked_image->setVisible(false);

		unchecked_image->setVisible(true);
	}
}
void BD_CheckBox::Update()
{
}
void BD_CheckBox::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
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
			SoundManager::Instance()->play_EffectMusic("sounds/check_box_on_click.mp3");
			if(!isChecked)
			{
				checked_image->setVisible(true);
				isChecked = true;
			}
			else
			{
				checked_image->setVisible(false);
				isChecked = false;
			}
			break;
		}

		set_Message(isChecked);
	}
}

void BD_CheckBox::Destroy()
{
	CCAssert(checked_image != nullptr, "BD_CheckBox : CheckedImage Already nullptr");
	CCAssert(unchecked_image != nullptr, "BD_CheckBox : UncheckedImage Already nullptr");

	removeChild(checked_image, true);
	removeChild(unchecked_image, true);
}
void* BD_CheckBox::send_message_main()
{
	return nullptr;
}
void* BD_CheckBox::send_message_lua()
{
	return nullptr;
}
void BD_CheckBox::recv_message_main(void* src_msg)
{
}

void BD_CheckBox::recv_message_lua(void* src_msg)
{
	bool isPreChecked = static_cast<bool>(src_msg);

	isChecked = isPreChecked;
	checked_image->setVisible(isChecked);
}

void BD_CheckBox::setResource(void* packet)
{
	chk_box_pack* chk_packet = static_cast<chk_box_pack*>(packet);

	setPosition(chk_packet->checked_image_info.image_pt);

	cocos2d::CCPoint checked_pos = cocos2d::CCPoint(0.0f, 0.0f);
	cocos2d::CCPoint unchecked_pos = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(checked_image, chk_packet->checked_image_info.file_name, &checked_pos);
	GraphicsController::Instance()->getSprite(unchecked_image, chk_packet->unchecked_image_info.file_name, 
							&unchecked_pos);
	
	checked_image->setVisible(false);
	unchecked_image->setVisible(true);

	checked_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	unchecked_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	checked_image->setZOrder(2);
	this->setContentSize(unchecked_image->getContentSize());

	addChild(checked_image);
	addChild(unchecked_image);

	checked_image->retain();
	unchecked_image->retain();

	SAFE_DELETE(chk_packet);
}