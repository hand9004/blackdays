#include "HUDCollection.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

HUDCollection::HUDCollection(void)
{
}


HUDCollection::~HUDCollection(void)
{
}

void HUDCollection::Init_HUD(obj_info& game_object_info)
{
	this->game_obj_info = &game_object_info;

	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Images/hud.plist");

	GraphicsController::Instance()->getSprite(hp_frame, cache->spriteFrameByName("hp_frame.png"));
	GraphicsController::Instance()->getSprite(hp_gauge, cache->spriteFrameByName("hp_gauge.png"));
	hp_frame->setOpacity(128);
	hp_gauge->setOpacity(128);
	
	hp_frame->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
	hp_gauge->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));

	ObjectController::Instance()->addChild(hp_frame);
	ObjectController::Instance()->addChild(hp_gauge);

	if(!game_obj_info->isEnemy)
	{
		GraphicsController::Instance()->getSprite(current_select, cache->spriteFrameByName("character_select.png"));
//		GraphicsController::Instance()->getSprite(attacked_select, cache->spriteFrameByName("target_select.png"));

		current_select->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
//		attacked_select->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));

	 	ObjectController::Instance()->addChild(current_select);
//		ObjectController::Instance()->addChild(attacked_select);
	}
}
void HUDCollection::Update_HUD()
{
	hp_update();

	if(!game_obj_info->isEnemy)
	{
		selected_update();

		attacked_update();
	}
}
void HUDCollection::Destroy_HUD()
{
	game_obj_info = nullptr;

	ObjectController::Instance()->removeChild(hp_frame, true);
	ObjectController::Instance()->removeChild(hp_gauge, true);
	ObjectController::Instance()->removeChild(current_select, true);
//	ObjectController::Instance()->removeChild(attacked_select, true);
}
void HUDCollection::hp_update()
{
	cocos2d::CCSize hp_ui_size = hp_gauge->getContentSize();

	float hp_percentage = (float)game_obj_info->health_point / (float)game_obj_info->max_health_point;

	float hp_X = game_obj_info->pos.x, hp_Y = game_obj_info->pos.y + current_image_size.height;
	hp_frame->setPosition(cocos2d::CCPoint(hp_X, hp_Y));
	hp_gauge->setPosition(cocos2d::CCPoint(hp_X, hp_Y));

	hp_gauge->setScaleX(hp_percentage);
}
void HUDCollection::selected_update()
{
	if(isSelectObject)
		current_select->setVisible(true);
	else
		current_select->setVisible(false);

	CCSize hp_frame_size = hp_frame->getContentSize();

	float select_X = game_obj_info->pos.x, select_Y = game_obj_info->pos.y + current_image_size.height + hp_frame_size.height;
	current_select->setPosition(cocos2d::CCPoint(select_X, select_Y));
}
void HUDCollection::attacked_update()
{
	if(isAttackedByEnemy)
	{
		isSelectObject = true;
//		attacked_select->setVisible(true);
	}
	else
	{
		isSelectObject = false;
//		attacked_select->setVisible(false);
	}

//	float select_X = game_obj_info->pos.x, select_Y = game_obj_info->pos.y + current_image_size.height;
//	attacked_select->setPosition(cocos2d::CCPoint(select_X, select_Y));
}