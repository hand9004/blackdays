#include "Effect.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"

Effect::Effect(void) : selected_index(0), isEffectOn(false), start_time(0), end_time(0), rand_pick_pos(0.f, 0.f)
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
		if (!eff_info->effect_set_list.empty())
		{
			unsigned int effect_set_size = eff_info->effect_set_list.size();

			for (unsigned int i = 0; i < effect_set_size; ++i)
			{
				eff_draw_data* eff_draw_dat = new eff_draw_data;

				eff_draw_dat->eff_set_info = eff_info->effect_set_list.at(i);

				eff_draw_dat->eff_sprFrame = cache->spriteFrameByName(eff_draw_dat->eff_set_info->key);

				effect_frame.push_back(eff_draw_dat);

				eff_draw_dat->eff_sprFrame->retain();
			}

			CCSpriteFrame* first_eff_frame = effect_frame.at(0)->eff_sprFrame;
			GraphicsController::Instance()->getSprite(effect_spr, first_eff_frame);
			effect_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
			effect_spr->setZOrder(1);
			effect_spr->setVisible(false);

			ObjectController::Instance()->addChild(effect_spr);

			effect_spr->retain();
		}
	}
}
void Effect::Update()
{
	unsigned int effect_draw_size = effect_frame.size();

	if (!effect_frame.empty())
	{
		if (selected_index >= 0 && selected_index < effect_draw_size)
		{
			eff_draw_data* eff_draw_iter = effect_frame.at(selected_index);

			if (isEffectOn)
				effect_spr->setVisible(true);
			else
				effect_spr->setVisible(false);

			float relative_dist_X = eff_draw_iter->eff_set_info->relative_distance.x, relative_dist_Y = eff_draw_iter->eff_set_info->relative_distance.y;

			if (isFlipX)
			{
				effect_spr->setFlipX(true);
				if (relative_dist_X >= 0)
					relative_dist_X = -relative_dist_X;
			}
			else
				effect_spr->setFlipX(false);

			if(eff_draw_iter->eff_set_info->draw_type == DRAW_TO_ME_RANDOM)
			{
				if (isEffectOn)
					effect_spr->setPosition(CCPoint(object_pos.x + relative_dist_X + rand_pick_pos.x, object_pos.y + relative_dist_Y + rand_pick_pos.y));
				else
					effect_spr->setPosition(CCPoint(object_pos.x + relative_dist_X, object_pos.y + relative_dist_Y));
			}
			else if (eff_draw_iter->eff_set_info->draw_type == DRAW_TO_TARGET)
				effect_spr->setPosition(CCPoint(target_pos.x + relative_dist_X, target_pos.y + relative_dist_Y));
			else
				effect_spr->setPosition(CCPoint(object_pos.x + relative_dist_X, object_pos.y + relative_dist_Y));

			effect_spr->setDisplayFrame(eff_draw_iter->eff_sprFrame);
		}
		else
			effect_spr->setVisible(false);
	}
}
void Effect::Destroy()
{
	ObjectController::Instance()->removeChild(effect_spr, true);
	vector_clear(effect_frame);
}
bool Effect::isEffectEnd()
{
	if (!effect_frame.empty())
	{
		unsigned int effect_draw_size = effect_frame.size();
		if (selected_index >= effect_draw_size)
			return true;
		else
			return false;
	}
	else
		return false;
}
void Effect::setEffectRandomSeed()
{
	srand(get_ms_onSystem());

	unsigned int w = obj_rect.getMaxX() - obj_rect.getMinX();
	unsigned int h = obj_rect.getMaxY() - obj_rect.getMinY();

	// 게임 내 오브젝트가 앵커 포인트를 (0.5, 0)으로 맞췄을 때의 기준으로
	// 랜덤으로 출력할 영역을 잡는다.
	rand_pick_pos = CCPoint(rand() % (w / 3), rand() % (h / 2));
	bool is_minus_x = false, is_minus_y = false;
	INTEGER_TO_BOOLEAN(is_minus_x, rand() % 2);
	INTEGER_TO_BOOLEAN(is_minus_y, rand() % 2);

	rand_pick_pos.x = (is_minus_x) ? -rand_pick_pos.x : rand_pick_pos.x;
	rand_pick_pos.y = (is_minus_y) ? -rand_pick_pos.y : rand_pick_pos.y;
}
void Effect::syncWithEffectTime(unsigned int limit_time_ms)
{
	if (isEffectOn)
	{
		unsigned long delta_time = end_time - start_time;

		if (delta_time >= limit_time_ms)
		{
			++selected_index;

			start_time = 0;
			end_time = 0;
		}
		else
			end_time = get_ms_onSystem();
	}
	else
	{
		selected_index = 0;

		start_time = get_ms_onSystem();
		end_time = get_ms_onSystem();
	}
}
void Effect::syncWithEffectFrameIndex(unsigned int frame_index)
{
	if (!effect_frame.empty())
	{
		unsigned int effect_draw_size = effect_frame.size();

		for (unsigned int i = 0; i < effect_draw_size; ++i)
		{
			eff_draw_data* effect_draw_iter = effect_frame.at(i);

			if (effect_draw_iter->eff_set_info->applying_index == frame_index)
			{
				isEffectOn = true;
				selected_index = i;
				break;
			}
			else
			{
				isEffectOn = false;
				selected_index = 0;
			}
		}
	}
}