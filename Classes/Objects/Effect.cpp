#include "Effect.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

Effect::Effect(void) : selected_index(0), isEffectOn(false), start_time(0), end_time(0)
{
}

Effect::~Effect(void)
{
}

void Effect::Init(effect_info* eff_info)
{
	if(eff_info != nullptr)
	{
		this->eff_info = eff_info;

		cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();

		cache->addSpriteFramesWithFile(eff_info->effect_list_name);
		unsigned int effect_set_size = eff_info->effect_set_list.size();

		for(unsigned int i = 0; i < effect_set_size; ++i)
		{
			eff_draw_data* eff_draw_dat = new eff_draw_data;

			eff_draw_dat->eff_set_info = eff_info->effect_set_list[i];

			eff_draw_dat->eff_sprFrame = cache->spriteFrameByName(eff_draw_dat->eff_set_info->key);

			effect_frame.push_back(eff_draw_dat);
		}

		CCSpriteFrame* first_eff_frame = effect_frame.at(0)->eff_sprFrame;
		GraphicsController::Instance()->getSprite(effect_spr, first_eff_frame);
		effect_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
		effect_spr->setVisible(false);

		ObjectController::Instance()->addChild(effect_spr);
	}
}
void Effect::Update()
{
	unsigned int effect_draw_size = effect_frame.size();
	bool isListEmpty = effect_frame.empty();

	if(!isListEmpty)
	{
		if(selected_index >= 0 && selected_index < effect_draw_size)
		{
			eff_draw_data* eff_draw_iter = effect_frame.at(selected_index);

			if(isEffectOn)
				effect_spr->setVisible(true);
			else
				effect_spr->setVisible(false);

			float relative_dist_X = eff_draw_iter->eff_set_info->relative_distance.x, relative_dist_Y = eff_draw_iter->eff_set_info->relative_distance.y;

			if(isFlipX)
			{
				effect_spr->setFlipX(true);
				if(relative_dist_X >= 0)
					relative_dist_X = -relative_dist_X;
			}
			else
				effect_spr->setFlipX(false);

			switch (eff_draw_iter->eff_set_info->draw_type)
			{
			case DRAW_TO_ME:
				effect_spr->setPosition(CCPoint(object_pos.x + relative_dist_X, object_pos.y + relative_dist_Y));
				break;
			case DRAW_TO_TARGET:
				effect_spr->setPosition(CCPoint(target_pos.x + relative_dist_X, target_pos.y + relative_dist_Y));
				break;
			}

			effect_spr->setDisplayFrame(eff_draw_iter->eff_sprFrame);
		}
	}
}
void Effect::Destroy()
{
	ObjectController::Instance()->removeChild(effect_spr, true);
	vector_clear(effect_frame);
}
bool Effect::isEffectEnd()
{
	unsigned int effect_draw_size = effect_frame.size();
	if(selected_index >= effect_draw_size)
		return true;
	else
		return false;
}
void Effect::syncWithEffectTime(unsigned int limit_time_ms)
{
	if(start_time <= 0)
		start_time = get_ms_onSystem();

	end_time = get_ms_onSystem();

	unsigned long delta_time = end_time - start_time;

	if(delta_time >= limit_time_ms)
	{
		++selected_index;

		start_time = 0;
		end_time = 0;
	}
}
void Effect::syncWithEffectFrameIndex(unsigned int frame_index)
{
	unsigned int effect_draw_size = effect_frame.size();

	for(unsigned int i = 0; i < effect_draw_size; ++i)
	{
		eff_draw_data* effect_draw_iter = effect_frame.at(i);

		if(effect_draw_iter->eff_set_info->applying_index == frame_index)
		{
			selected_index = i;
			break;
		}
	}
}