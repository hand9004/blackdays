#include "Animate.h"
#include "ObjectController.h"
#include "../Graphics/GraphicsController.h"
#include "../Utility/Utility.h"

USING_NS_CC;

#include <algorithm>
Animate::Animate(void) : canSendMessage(false), isAnimateEnd(false), ani_index(0)
{
	start_time = get_ms_onSystem();
	end_time = get_ms_onSystem();
}


Animate::~Animate(void)
{
}

bool sort_animation_index(in_aniframe_info* fir, in_aniframe_info* sec)
{
	return fir->frame < sec->frame;
}

void Animate::Init(main_grap_info& game_graphic_info)
{
	effect_info* p_ani_in_effect = game_graphic_info.m_ani_info->effect_data;
	aniframe_set_info* p_ani_frame_set = game_graphic_info.m_ani_info->ani_frame_set;
	aniframe_info* p_ani_frame = game_graphic_info.m_ani_info->ani_frame_data;
	ani_name = p_ani_frame->ani_name;
	isOnePlay = p_ani_frame_set->isOneTimePlay;

	std::sort(p_ani_frame->in_aniframe_list.begin(), p_ani_frame->in_aniframe_list.end(), sort_animation_index);

	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(p_ani_frame->ref_list_name);
	
	unsigned int in_ani_frame_size = p_ani_frame->in_aniframe_list.size();
	for(unsigned int i = 0; i < in_ani_frame_size; ++i)
	{
		in_aniframe_info* in_aniframe_iter = p_ani_frame->in_aniframe_list[i];

		cocos2d::CCSpriteFrame* anim_frame = cache->spriteFrameByName(in_aniframe_iter->in_list_key);

		CCAssert(anim_frame != nullptr, "Can't AnimationFrame Insert To List. anim_frame Is nullptr.");

		anim_frame_list.push_back(anim_frame);

		BD_CCLog("%d %s %f %f %f %f", in_aniframe_iter->frame, in_aniframe_iter->in_list_key,
			anim_frame->getRect().getMinX(),  anim_frame->getRect().getMinY(), anim_frame->getRect().getMaxX(), anim_frame->getRect().getMaxY());
	}

	set_Delay(p_ani_frame_set->delay);
	set_Repeat_Area(p_ani_frame_set->rep_start, p_ani_frame_set->rep_end);

	GraphicsController::Instance()->getSprite(draw_spr, anim_frame_list.at(0));
	draw_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));

	ObjectController::Instance()->addChild(draw_spr);

	grap_effect.Init(p_ani_in_effect);
}
void Animate::Update()
{
	bool isSpriteVisible = draw_spr->isVisible();

	check_AnimationEnd();

	if(isSpriteVisible)
		animation_Play();
	else
		animation_Stop();

	grap_effect.setTargetPos(target_pos);
	grap_effect.setEffectVisible(isSpriteVisible);
	grap_effect.setObjectPos(draw_spr->getPosition());
	grap_effect.syncWithEffectFrameIndex(ani_index);
	grap_effect.Update();

	setCollisionRect();
}
void Animate::Destroy()
{
	vector_clear(anim_frame_list);
	ObjectController::Instance()->removeChild(draw_spr, true);
	grap_effect.Destroy();
}
void Animate::SetPacket(grap_to_obj_packet& src_packet)
{
	grap_to_obj_animate *send_ani_packet = new grap_to_obj_animate;
	send_ani_packet->current_index = ani_index;
	send_ani_packet->isAnimationFinish = isAnimateEnd;
	src_packet.data = (void*)send_ani_packet;
}
void Animate::animation_Play()
{
	bool isChangeAnimationFrame = false;
	unsigned int aniframe_size = anim_frame_list.size();
	long delta_time = end_time - start_time;
	if (delta_time > (frame_interval_time * 1000))
	{
		// 애니메이션의 반복 구역을 정했을 경우와 안 정했을 경우의 처리.
		if (rep_start_index > 0 && rep_end_index > 0)
		{
			if (ani_index < rep_end_index)
				isChangeAnimationFrame = true;
			else
			{
				if (!isOnePlay)
					ani_index = rep_start_index;
			}
		}
		else
		{
			if (ani_index < aniframe_size)
				isChangeAnimationFrame = true;
			else
			{
				if (!isOnePlay)
					ani_index = 0;
			}
		}
		start_time = get_ms_onSystem();
		end_time = get_ms_onSystem();
		canSendMessage = true;
	}
	else
	{
		canSendMessage = false;
		end_time = get_ms_onSystem();
	}

	if(isChangeAnimationFrame)
		draw_spr->setDisplayFrame(anim_frame_list.at(ani_index++));
}
void Animate::animation_Stop()
{
	start_time = get_ms_onSystem();
	end_time = get_ms_onSystem();
	ani_index = 0;
}
void Animate::check_AnimationEnd()
{
	unsigned int anim_max_size = anim_frame_list.size();
	bool isAnimationEnd_Index = (ani_index >= anim_frame_list.size()) ? true : false;

	if(isAnimationEnd_Index)
	{
		if(canSendMessage)
			isAnimateEnd = true;
		else
			isAnimateEnd = false;
	}
	else
		isAnimateEnd = false;
}
void Animate::set_Repeat_Area(unsigned int rep_start, unsigned int rep_end)
{
	rep_start_index = rep_start;
	rep_end_index = rep_end;
}
void Animate::set_Delay(float delay)
{
	frame_interval_time = delay;
}
void Animate::setCollisionRect()
{
	unsigned int anim_size = anim_frame_list.size();
	if(ani_index < anim_size)
	{
		cocos2d::CCRect image_rect = anim_frame_list.at(ani_index)->getRect();
		int image_width = image_rect.getMaxX() - image_rect.getMinX();
		int image_height = image_rect.getMaxY() - image_rect.getMinY();
		cocos2d::CCPoint spr_pos = draw_spr->getPosition();
		cocos2d::CCRect collision_rect = cocos2d::CCRect(spr_pos.x - (image_width / 2), spr_pos.y, image_width, image_height);
		setContentRect(collision_rect);
	}
}