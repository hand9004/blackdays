#include "Normal.h"
#include "ObjectProtocol.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

Normal::Normal(void)
{
}


Normal::~Normal(void)
{
}

void Normal::Init(main_grap_info& game_graphic_info)
{
	cocos2d::CCSpriteBatchNode* batch_pt = cocos2d::CCSpriteBatchNode::create(game_graphic_info.m_imgframe_info->base_image);
	
	cocos2d::CCRect img_area = game_graphic_info.m_imgframe_info->in_image_area;
	GraphicsController::Instance()->getSprite(draw_spr, batch_pt->getTexture());
	draw_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
	batch_pt->addChild(draw_spr);

	ObjectController::Instance()->addChild(batch_pt);
}
void Normal::Update()
{

}
void Normal::Destroy()
{
	ObjectController::Instance()->removeChild(draw_spr, true);
	grap_effect.Destroy();
}
void Normal::SetPacket(grap_to_obj_packet& src_packet)
{
}