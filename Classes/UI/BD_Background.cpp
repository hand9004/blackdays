#include "BD_Background.h"
#include "UIController.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"

BD_Background::BD_Background(void) : background_image(nullptr)
{
}


BD_Background::~BD_Background(void)
{
}

void BD_Background::Init(void* packet)
{
	setResource(packet);
}

bool BD_Background::get_UIActive()
{
	return background_image->isVisible();
}

void BD_Background::set_UIActive(bool isActive)
{
	if(!isActive)
		background_image->setVisible(false);
	else
		background_image->setVisible(true);
}
void BD_Background::Update()
{
}
void BD_Background::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint t_pt)
{
//	set_UIStatus(TOUCH_NO_EVENT);
}

void BD_Background::Destroy()
{
	CCAssert(background_image != nullptr, "BD_Background : BackgroundImage Already nullptr");

	removeChild(background_image, true);
}
void* BD_Background::send_message_main()
{
	return nullptr;
}
void* BD_Background::send_message_lua()
{
	return nullptr;
}
void BD_Background::recv_message_main(void* src_msg)
{
	bg_msg_recv* recv_msg = static_cast<bg_msg_recv*>(src_msg);

	CCAssert(recv_msg != nullptr, "BD_Background recv_message_main packet is nullptr!");

	CCSprite* change_img_spr = CCSprite::create(recv_msg->change_image_path);
	CCTexture2D* change_img_tex = change_img_spr->getTexture();

	CCSize img_size = change_img_tex->getContentSize();
	background_image->setTexture(change_img_tex);
	background_image->setTextureRect(CCRect(0, 0, img_size.width, img_size.height));
	background_image->setContentSize(img_size);

	change_img_spr->retain();
}

void BD_Background::recv_message_lua(void* src_msg)
{
}

void BD_Background::setResource(void* packet)
{
	bg_pack* butt_packet = static_cast<bg_pack*>(packet);

	setPosition(butt_packet->background_image_info.image_pt);

	cocos2d::CCPoint background_pt = cocos2d::CCPoint(0.0f, 0.0f);

	GraphicsController::Instance()->getSprite(background_image, butt_packet->background_image_info.file_name,
						&background_pt);

	background_image->setVisible(true);
	background_image->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));

	this->setContentSize(background_image->getContentSize());

	addChild(background_image);

	background_image->retain();

	SAFE_DELETE(butt_packet);
}