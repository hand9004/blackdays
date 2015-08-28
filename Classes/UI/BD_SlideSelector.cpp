#include "BD_SlideSelector.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"

BD_SlideSelector::BD_SlideSelector(void) : isVertical(true), current_index(0), list_size(0), latest_slided_dir_vec(0.0f, 0.0f), 
											background_image(nullptr), talk_balloon_image(nullptr)
{
}

BD_SlideSelector::~BD_SlideSelector(void)
{
}

void BD_SlideSelector::Init(void* packet)
{
	setResource(packet);
}
bool BD_SlideSelector::get_UIActive()
{
	return background_image->isVisible();
}
void BD_SlideSelector::set_UIActive(bool isActive)
{
	if(!isActive)
	{
		background_image->setVisible(false);
		talk_balloon_image->setVisible(false);
	}
	else
	{
		background_image->setVisible(true);
		talk_balloon_image->setVisible(true);
	}
}
void BD_SlideSelector::Update()
{
}
void BD_SlideSelector::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
{
	set_UIStatus(t_event);

	float touch_delta_X = 0.0f, touch_delta_Y = 0.0f;

	bool isTouchEnable = this->isTouchEnabled();
	if(isTouchEnable)
	{
		switch(t_event)
		{
		case TOUCH_BEGIN:
			touch_begin_pt = t_pt;
			isUISelected = true;
			break;
		case TOUCH_MOVED:
			break;
		case TOUCH_ENDED:
			if(isVertical)
			{
				touch_delta_Y = t_pt.y - touch_begin_pt.y;

				if (touch_delta_Y > swipe_disable_range)
				{
					if(current_index > 0)
						--current_index;
					else
						current_index = 0;

					isSwipeMode = true;
				}
				else if (touch_delta_Y < -swipe_disable_range)
				{
					if(current_index < list_size - 1)
						++current_index;
					else
						current_index = list_size - 1;

					isSwipeMode = true;
				}
				else
					isSwipeMode = false;
			}
			else
			{
				touch_delta_X = t_pt.x - touch_begin_pt.x;

				if (touch_delta_X > swipe_disable_range)
				{
					if(current_index < list_size - 1)
						++current_index;
					else
						current_index = list_size - 1;

					isSwipeMode = true;
				}
				else if (touch_delta_X < -swipe_disable_range)
				{
					if(current_index > 0)
						--current_index;
					else
						current_index = 0;

					isSwipeMode = true;
				}
				else
					isSwipeMode = false;
			}

			latest_slided_dir_vec.x = touch_delta_X;
			latest_slided_dir_vec.y = touch_delta_Y;

			set_Message(current_index);
			isUISelected = false;
			break;
		}
	}
}
void BD_SlideSelector::Destroy()
{
	CCAssert(background_image != nullptr, "BD_SlideSelector : BackgroundImage Already nullptr");
	CCAssert(talk_balloon_image != nullptr, "BD_SlideSelector : TalkBalloonImage Already nullptr");

	removeChild(background_image, true);
	removeChild(talk_balloon_image, true);
}

void* BD_SlideSelector::send_message_main()
{
	slidesel_msg_send* send_message = new slidesel_msg_send;

	cocos2d::CCPoint global_pos = getPosition();
	cocos2d::CCSize contents_size = this->getContentSize();
	cocos2d::CCSize balloon_size = talk_balloon_image->getContentSize();
	float talk_balloon_pos_X = 0.0f, talk_balloon_pos_Y = 0.0f;

	if(isVertical)
	{
		talk_balloon_pos_X = global_pos.x + contents_size.width;
		talk_balloon_pos_Y = global_pos.y + (balloon_size.height * 0.5);

		cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();
		if(talk_balloon_pos_X + balloon_size.width > win_size.width)
			talk_balloon_pos_X = global_pos.x - (balloon_size.width * 0.5);
	}
	else
	{
	}

	send_message->isVertical = isVertical;
	send_message->current_index = current_index;
	send_message->slide_direction_vec = latest_slided_dir_vec;
	send_message->linked_element_pos = cocos2d::CCPoint(talk_balloon_pos_X, talk_balloon_pos_Y);

	return send_message;
}
void* BD_SlideSelector::send_message_lua()
{
	return nullptr;
}

void BD_SlideSelector::recv_message_main(void* src_msg)
{
	slidesel_msg_recv* recv_message = static_cast<slidesel_msg_recv*>(src_msg);

	list_size = recv_message->list_size;

	if(recv_message->current_index >= 0)
		current_index = recv_message->current_index;

	if(current_index >= list_size)
		current_index = 0;
	else if(current_index < 0)
		current_index = 0;

	set_Message(current_index);
}

void BD_SlideSelector::recv_message_lua(void* src_msg)
{
}

void BD_SlideSelector::setResource(void* packet)
{
	slide_sel_pack* slide_sel_packet = static_cast<slide_sel_pack*>(packet);

	isVertical = slide_sel_packet->isVertical;

	this->setPosition(slide_sel_packet->background_image_info.image_pt);

	cocos2d::CCPoint background_pos = cocos2d::CCPoint(0.0f, 0.0f);
	cocos2d::CCPoint talk_balloon_pos = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(background_image, slide_sel_packet->background_image_info.file_name, 
							&background_pos);
	GraphicsController::Instance()->getSprite(talk_balloon_image, slide_sel_packet->talk_balloon_info.file_name, 
							&talk_balloon_pos);

	this->setContentSize(background_image->getContentSize());

	cocos2d::CCSize contents_size = this->getContentSize();
	cocos2d::CCSize balloon_size = talk_balloon_image->getContentSize();
	cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();

	if(isVertical)
	{
		cocos2d::CCPoint ui_pos = getPosition();
		float talk_balloon_pos_X = background_pos.x + contents_size.width, talk_balloon_pos_Y = background_pos.y;

		if(ui_pos.x + talk_balloon_pos_X + balloon_size.width > win_size.width)
			talk_balloon_pos_X -= talk_balloon_pos_X + balloon_size.width;

		talk_balloon_pos = cocos2d::CCPoint(talk_balloon_pos_X, talk_balloon_pos_Y);
	}
	else
	{
		background_image->setRotation(90.0f);
		talk_balloon_pos = cocos2d::CCPoint(background_pos.x + (contents_size.width * 0.4), background_pos.y + contents_size.height);
	}

	background_image->setPosition(background_pos);
	talk_balloon_image->setPosition(talk_balloon_pos);

	background_image->setVisible(true);
	talk_balloon_image->setVisible(true);

	background_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	talk_balloon_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	addChild(background_image);
	addChild(talk_balloon_image);

	BD_CCLog("%f %f %f %f", background_pos.x, background_pos.y, talk_balloon_pos.x, talk_balloon_pos.y);
}