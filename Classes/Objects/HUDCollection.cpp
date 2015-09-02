#include "HUDCollection.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

HUDCollection::HUDCollection(void) : onRun(false), onHide(false),
hp_frame(nullptr), hp_gauge(nullptr), current_select(nullptr), move_select(nullptr), hide_frame(nullptr), run_frame(nullptr), sleep_frame(nullptr)
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

	float hp_X = game_obj_info->pos.x, hp_Y = game_obj_info->pos.y + current_image_size.height;
	hp_frame->setPosition(cocos2d::CCPoint(hp_X, hp_Y));
	hp_gauge->setPosition(cocos2d::CCPoint(hp_X, hp_Y));

	ObjectController::Instance()->addChild(hp_frame);
	ObjectController::Instance()->addChild(hp_gauge);

	if(!game_obj_info->isEnemy)
	{
		GraphicsController::Instance()->getSprite(current_select, cache->spriteFrameByName("arrow.png"));
		GraphicsController::Instance()->getSprite(move_select, cache->spriteFrameByName("move_arrow.png"));
		GraphicsController::Instance()->getSprite(run_frame, cache->spriteFrameByName("run_icon.png"));
		GraphicsController::Instance()->getSprite(hide_frame, cache->spriteFrameByName("hide_icon.png"));

		CCSize hp_frame_size = hp_frame->getContentSize();
		float select_X = game_obj_info->pos.x, select_Y = game_obj_info->pos.y + current_image_size.height + hp_frame_size.height;
		current_select->setPosition(cocos2d::CCPoint(select_X, select_Y));

		current_select->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
		move_select->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
		run_frame->setAnchorPoint(cocos2d::CCPoint(0.5, 0.0f));
		hide_frame->setAnchorPoint(cocos2d::CCPoint(0.5, 0.0f));

	 	ObjectController::Instance()->addChild(current_select);
		ObjectController::Instance()->addChild(move_select);
		ObjectController::Instance()->addChild(run_frame);
		ObjectController::Instance()->addChild(hide_frame);
	}
	else
	{
		GraphicsController::Instance()->getSprite(sleep_frame, cache->spriteFrameByName("sleep.png"));

		float sleep_X = game_obj_info->pos.x, sleep_Y = game_obj_info->pos.y + current_image_size.height;
		sleep_frame->setPosition(CCPoint(sleep_X, sleep_Y));
		sleep_frame->setAnchorPoint(cocos2d::CCPoint(0.5, 0.0f));

		ObjectController::Instance()->addChild(sleep_frame);
	}
}
void HUDCollection::Update_HUD()
{
	hp_update();

	status_update();

	if(!game_obj_info->isEnemy)
	{
		selected_update();

		attacked_update();
	}
}
void HUDCollection::Update_ScrollingPos_HUD(float delta_x)
{
	move_point.x += delta_x;
}
void HUDCollection::Destroy_HUD()
{
	game_obj_info = nullptr;

	ObjectController::Instance()->removeChild(hp_frame, true);
	ObjectController::Instance()->removeChild(hp_gauge, true);
	ObjectController::Instance()->removeChild(current_select, true);
	ObjectController::Instance()->removeChild(move_select, true);
	ObjectController::Instance()->removeChild(run_frame, true);
	ObjectController::Instance()->removeChild(hide_frame, true);
	ObjectController::Instance()->removeChild(sleep_frame, true);
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
	//if(isAttackedByEnemy)
	//{
	//	isSelectObject = true;
	//	attacked_select->setVisible(true);
	//}
	//else
	//{
	//	isSelectObject = false;
	//	attacked_select->setVisible(false);
	//}

	//float select_X = game_obj_info->pos.x, select_Y = game_obj_info->pos.y + current_image_size.height;
	//attacked_select->setPosition(cocos2d::CCPoint(select_X, select_Y));
}
void HUDCollection::status_update()
{
	cocos2d::CCSize win_size = CCDirector::sharedDirector()->getWinSize();
	unsigned int status_cnt = 0;

	if (!game_obj_info->isEnemy)
	{
		cocos2d::CCPoint draw_pt = CCPoint(win_size.width * 0.5, win_size.height - 50);
		if (onRun)
		{
			run_frame->setVisible(true);
			run_frame->setPosition(draw_pt);
			++status_cnt;
		}
		else
			run_frame->setVisible(false);

		if (onHide)
		{
			hide_frame->setVisible(true);
			CCSize hide_icon_size = hide_frame->getContentSize();
			draw_pt.x += hide_icon_size.width * status_cnt;
			hide_frame->setPosition(draw_pt);
			++status_cnt;
		}
		else
			hide_frame->setVisible(false);

		switch (curr_ev)
		{
		case MOVE:
			move_select->setVisible(true);
			move_select->setPosition(move_point);
			break;
		default:
			move_select->setVisible(false);
			break;
		}
	}
	else
	{
		float sleep_X = game_obj_info->pos.x, sleep_Y = game_obj_info->pos.y + current_image_size.height;
		switch(curr_ev)
		{
		case SLEEP:
			sleep_frame->setVisible(true);
			sleep_frame->setPosition(CCPoint(sleep_X, sleep_Y));
			break;
		default:
			sleep_frame->setVisible(false);
			break;
		}
	}
}