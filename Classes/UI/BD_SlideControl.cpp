#include "BD_SlideControl.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"

#include <iomanip>

BD_SlideControl::BD_SlideControl(void) : currentSlidedValue(0), bar_x_mid_val(0), background_image(nullptr), slide_bar_image(nullptr)
										 ,test_font(nullptr)
{
}


BD_SlideControl::~BD_SlideControl(void)
{
}

void BD_SlideControl::Init(void* packet)
{
	setResource(packet);
}

bool BD_SlideControl::get_UIActive()
{
	return (background_image->isVisible() || slide_bar_image->isVisible());
}

void BD_SlideControl::set_UIActive(bool isActive)
{
	if(!isActive)
	{
		background_image->setVisible(false);
		slide_bar_image->setVisible(false);
#ifdef DEBUG_MODE
		test_font->setVisible(false);
#endif
	}
	else
	{
		background_image->setVisible(true);
		slide_bar_image->setVisible(true);
#ifdef DEBUG_MODE
		test_font->setVisible(true);
#endif
	}
}
void BD_SlideControl::Update()
{
}
void BD_SlideControl::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
{
	set_UIStatus(t_event);

	bool is_touch_enable = this->isTouchEnabled();
	if(is_touch_enable)
	{
		char ret[256] = {0,};
		unsigned int field_data = 0;
		float rounded_slide_val = 0.0f;
		switch(t_event)
		{
		case TOUCH_BEGIN:
			isUISelected = true;
		case TOUCH_MOVED:
			if(isUISelected)
			{
				cocos2d::CCSize contents_size = getContentSize();
				cocos2d::CCPoint background_pos = getPosition();
				slide_bar_pos.x = (t_pt.x - background_pos.x) - bar_x_mid_val;
				slide_bar_image->setPositionX(slide_bar_pos.x);
				slide_bar_pos.x += bar_x_mid_val;

				slide_bar_real_pos = cocos2d::CCPoint(background_pos.x + slide_bar_pos.x, background_pos.y + slide_bar_pos.y);
				rounded_slide_val = (((slide_bar_real_pos.x - background_pos.x) / (contents_size.width)) * 100) + 0.5;
				currentSlidedValue = (int)rounded_slide_val;

				set_Message(currentSlidedValue); 
			}
			break;
		case TOUCH_ENDED:
			isUISelected = false;
			break;
		}
#ifdef DEBUG_MODE
		std::stringstream temp;
		temp << std::setw(1) << currentSlidedValue;
		std::string str = temp.str();
		test_font->setString(str.c_str());
#endif
	}
}

void BD_SlideControl::Destroy()
{
	CCAssert(background_image != nullptr, "BD_SlideControl : BackgroundImage Already nullptr");
	CCAssert(slide_bar_image != nullptr, "BD_SlideControl : SlideBarImage Already nullptr");

	removeChild(background_image, true);
	removeChild(slide_bar_image, true);
}

void* BD_SlideControl::send_message_main()
{
	return nullptr;
}
void* BD_SlideControl::send_message_lua()
{
	return nullptr;
}

void BD_SlideControl::recv_message_main(void* src_msg)
{
}

void BD_SlideControl::recv_message_lua(void* src_msg)
{
	unsigned int slided_val = (unsigned int)(src_msg);

	currentSlidedValue = slided_val;

	cocos2d::CCSize contents_size = getContentSize();

	float scale_value = (currentSlidedValue * 0.01f);
	float real_pos_x = (contents_size.width * scale_value);
	slide_bar_pos.x = real_pos_x - bar_x_mid_val;
	slide_bar_image->setPositionX(slide_bar_pos.x);
	slide_bar_pos.x += bar_x_mid_val;

#ifdef DEBUG_MODE
	std::stringstream temp;
	temp << std::setw(1) << currentSlidedValue;
	std::string str = temp.str();
	test_font->setString(str.c_str());
#endif

	set_Message(currentSlidedValue);
}

void BD_SlideControl::setResource(void* packet)
{
	slide_ctrl_pack* slide_packet = static_cast<slide_ctrl_pack*>(packet);

	setPosition(slide_packet->background_image_info.image_pt);

	cocos2d::CCPoint background_pos = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(background_image, slide_packet->background_image_info.file_name, 
							&background_pos);
	GraphicsController::Instance()->getSprite(slide_bar_image, slide_packet->slide_bar_info.file_name, 
							&background_pos);

	background_image->setVisible(true);
	slide_bar_image->setVisible(true);

	background_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	slide_bar_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	slide_bar_pos.x = background_image->getPosition().x;
	slide_bar_pos.y = background_image->getPosition().y;

	bar_x_mid_val = slide_bar_image->getContentSize().width / 2;
	
	slide_bar_image->setPosition(cocos2d::CCPoint(slide_bar_pos.x - bar_x_mid_val, slide_bar_pos.y));

	this->setContentSize(background_image->getContentSize());
	cocos2d::CCSize contents_size = this->getContentSize();

	ui_rect = cocos2d::CCRect(background_pos.x, background_pos.y, contents_size.width, contents_size.height);

	addChild(background_image);
	addChild(slide_bar_image);

#ifdef DEBUG_MODE
	test_font = cocos2d::CCLabelBMFont::create();
	test_font->setFntFile("fonts/arial-unicode-26.fnt");
	test_font->setPosition(cocos2d::CCPoint(background_pos.x + contents_size.width, background_pos.y)); 
	test_font->setColor(cocos2d::ccc3(255, 255, 255));
	test_font->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	test_font->setString("0");
	
	addChild(test_font);
#endif

	SAFE_DELETE(slide_packet);
}