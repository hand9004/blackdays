#include "CurveThrowObject.h"
#include "../Utility/Utility.h"
#include "ObjectController.h"
#include "../SoundManager.h"
#include "../Graphics/GraphicsController.h"

const float object_speed = 10.0f;

CurveThrowObject::CurveThrowObject(void) : current_index(0), isBombEffectEnd(false), isSoundPlayed(false), isSendedDamage(false)
{
}

CurveThrowObject::~CurveThrowObject(void)
{
}

void CurveThrowObject::Init(cocos2d::CCPoint start_pt, cocos2d::CCPoint end_pt)
{
	radius_X = 50.0f, radius_Y = 20.0f;

	create_bezier_to_pos(start_pt, end_pt);
	GraphicsController::Instance()->getSprite(curve_throw_spr, "Images/shell.png", &start_pt);
	curve_throw_spr->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
	ObjectController::Instance()->addChild(curve_throw_spr);

	effect_info* effect_info_data = new effect_info;

	effect_info_data->effect_list_name = "Images/explosion_1.plist";

//	effect_frame_name_list.push_back("explosion_1.png");
	effect_frame_name_list.push_back("explosion_2.png");
	effect_frame_name_list.push_back("explosion_3.png");
	effect_frame_name_list.push_back("explosion_4.png");
	effect_frame_name_list.push_back("explosion_5.png");
	effect_frame_name_list.push_back("explosion_6.png");
	effect_frame_name_list.push_back("explosion_7.png");
	effect_frame_name_list.push_back("explosion_8.png");
	effect_frame_name_list.push_back("explosion_9.png");
	effect_frame_name_list.push_back("explosion_10.png");
	effect_frame_name_list.push_back("explosion_11.png");
	effect_frame_name_list.push_back("explosion_12.png");
	effect_frame_name_list.push_back("explosion_13.png");
	effect_frame_name_list.push_back("explosion_14.png");
	effect_frame_name_list.push_back("explosion_15.png");

	unsigned int effect_frame_size = effect_frame_name_list.size();
	for(unsigned int i = 0; i < effect_frame_size; ++i)
	{
		effect_set_info* effect_frame_data = new effect_set_info;

		effect_frame_data->applying_index = 0;
		effect_frame_data->key = effect_frame_name_list.at(i).c_str();
		effect_frame_data->relative_distance = cocos2d::CCPoint(0.0f, 0.0f);

		effect_info_data->effect_set_list.push_back(effect_frame_data);
	}

	bomb_explosion_effect.Init(effect_info_data);

	vector_clear(effect_frame_name_list);
}
void CurveThrowObject::Update()
{
	unsigned int throwing_size = throwing_line.size();
	if(!throwing_line.empty())
	{
		if(current_index >= 0 && current_index < throwing_size)
		{
			cocos2d::CCPoint current_pos = throwing_line.at(current_index);

			curve_throw_spr->setPosition(current_pos);
			++current_index;
		}
		else
		{
			cocos2d::CCPoint current_pos = throwing_line.at(throwing_line.size() - 1);
			curve_throw_spr->setPosition(current_pos);

			if(!isSoundPlayed)
			{
				SoundManager::Instance()->play_EffectMusic("sounds/bomb_explosion.mp3");
				isSoundPlayed = true;
			}
		}
	}

	if(current_index >= throwing_size)
	{
		curve_throw_spr->setVisible(false);
		bomb_explosion_effect.setObjectPos(curve_throw_spr->getPosition());
		if(!bomb_explosion_effect.isEffectEnd())
		{
			bomb_explosion_effect.setEffectVisible(true);
			bomb_explosion_effect.syncWithEffectTime(15);
			bomb_explosion_effect.Update();
		}
		else
		{
			bomb_explosion_effect.setEffectVisible(false);
			isBombEffectEnd = true;
		}
	}
}
void CurveThrowObject::Destroy()
{
	bomb_explosion_effect.Destroy();
	ObjectController::Instance()->removeChild(curve_throw_spr, true);
	vector_clear(throwing_line);
	vector_clear(effect_frame_name_list);
}
void CurveThrowObject::change_PosList_OnScrolling(float delta_X)
{
	unsigned int throw_pos_size = throwing_line.size();
	for(unsigned int i = 0; i < throw_pos_size; ++i)
		throwing_line.at(i).x += delta_X;
}
bool CurveThrowObject::isInBombArea(cocos2d::CCPoint src_pos)
{
	unsigned int throw_size = throwing_line.size();
	cocos2d::CCPoint destroyed_pt = throwing_line.at(throw_size - 1);
	cocos2d::CCRect destroyed_area = cocos2d::CCRect(destroyed_pt.x - radius_X, destroyed_pt.y - radius_Y, radius_X * 2, radius_Y * 2);

	if(destroyed_area.containsPoint(src_pos))
		return true;
	else
		return false;
}
void CurveThrowObject::create_bezier_to_pos(cocos2d::CCPoint start_pt, cocos2d::CCPoint end_pt)
{
	float radient = (end_pt.y - start_pt.y) / (end_pt.x - start_pt.x);
	float atan_val_radient = atan(radient);

	float degree_30 = (float)(30.0f * (PI / 180.0f));
	float atan_val_degree_30 = atan(degree_30);
	
	float degree_90 = (float)(90.0f * (PI / 180.0f));
	float atan_val_degree_90 = atan(degree_90);

	float atan_vertical_of_radient = atan_val_radient + atan_val_degree_90;
	float total_atan_val = atan_val_radient + atan_val_degree_30;

	float line_size_X = (end_pt.x > start_pt.x) ? end_pt.x - start_pt.x : start_pt.x - end_pt.x;
	float line_size_Y = (end_pt.y > start_pt.y) ? end_pt.y - start_pt.y : start_pt.y - end_pt.y;

	float mid_X = (end_pt.x > start_pt.x) ? start_pt.x - (line_size_X / 2) : end_pt.x + (line_size_X / 2);
	float mid_Y = (end_pt.y > start_pt.y) ? start_pt.y + (line_size_Y / 2) : end_pt.y + (line_size_Y / 2);

	float dist_X = end_pt.x - start_pt.x, dist_Y = end_pt.y - start_pt.y;
	float dist_of_start_to_end = sqrt(dist_X * dist_X + dist_Y * dist_Y);

	float dist_start_to_mid_X = mid_X - start_pt.x, dist_start_to_mid_Y = mid_Y - start_pt.y;
	float dist_of_start_to_mid = sqrt((dist_start_to_mid_X * dist_start_to_mid_X) + (dist_start_to_mid_Y * dist_start_to_mid_Y));
	float sel_X = dist_of_start_to_mid * cos(atan_vertical_of_radient), sel_Y = dist_of_start_to_mid * sin(atan_vertical_of_radient);

	cocos2d::CCPoint selected_pt = cocos2d::CCPoint(mid_X + sel_X, mid_Y + sel_Y);

	float bezier_step = object_speed / dist_of_start_to_end;
	float step_t = 0.0f;

	while(step_t < 1.0f)
	{
		float one_minus_t = 1 - step_t;
		float t_square = step_t * step_t;
		float two_mul_t = 2 * step_t;

		float step_X = (one_minus_t * one_minus_t * start_pt.x) + (two_mul_t * one_minus_t * selected_pt.x) + (t_square * end_pt.x);
		float step_Y = (one_minus_t * one_minus_t * start_pt.y) + (two_mul_t * one_minus_t * selected_pt.y) + (t_square * end_pt.y);

		throwing_line.push_back(cocos2d::CCPoint(step_X, step_Y));

		step_t += bezier_step;
	}
}