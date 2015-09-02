#include "SkillController.h"
#include "../UI/UIController.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"
#include "../Utility/Utility.h"

#include <algorithm> 

const unsigned int opacity_limit = 255;
const unsigned int hide_opacity_speed = 2;
const float hide_opacity_limit = 0.2f;

SkillController::SkillController(void) : isSkillFinish(false), onRun(false), onHide(false), current_skill(0), chain_index(0), hide_start_time(0), hide_end_time(0),
											run_start_time(0), run_end_time(0), target_obj_info(nullptr), game_obj_info(nullptr), obj_spr(nullptr), game_obj_ev(nullptr), g_ctrl(nullptr)
{
	hide_start_time = get_ms_onSystem();
	hide_end_time = get_ms_onSystem();

	run_start_time = get_ms_onSystem();
	run_end_time = get_ms_onSystem();
}

SkillController::~SkillController(void)
{
}

void SkillController::Init(obj_info& object_info)
{
	game_obj_info = &object_info;

	unsigned int skill_size = game_obj_info->skill_list.size();

	skill_cnt = skill_size;
}
void SkillController::Update()
{	
	if (game_obj_ev != nullptr)
	{
		updateRun();
		updateHide();

		if (*game_obj_ev == SKILL)
		{
			unsigned int skill_size = game_obj_info->skill_list.size();
			if (current_skill >= 0 && current_skill < skill_size)
			{
				skill_info* skill_iter = game_obj_info->skill_list[current_skill];

				const char* skill_type = skill_iter->skill_type;
				if (!strcmp(skill_type, "Charge"))
					updateCharge();
				else if (!strcmp(skill_type, "Power"))
					updatePower();
				else if (!strcmp(skill_type, "Chain"))
					updateChain();
			}
			if (isSkillFinish)
			{
				*game_obj_ev = SEARCHING_RECOGNIZE_AREA;
				chain_index = 0;
			}
		}
	}
}
void SkillController::Update_UI()
{
}
void SkillController::Destroy()
{
	game_obj_ev = nullptr, game_obj_info = nullptr, target_obj_info = nullptr;
}
void SkillController::update_Scrolling(float delta_x)
{
	if(!game_obj_info->skill_list.empty())
	{
		skill_info* skill_iter = game_obj_info->skill_list[current_skill];

		const char* skill_type_name = skill_iter->skill_type;
		if(!strcmp(skill_type_name, "Charge"))
		{
			start_player_pt.x += delta_x;
			end_player_pt.x += delta_x;
		}
	}
}
void SkillController::updateCharge()
{
	skill_info* skill_iter = game_obj_info->skill_list[current_skill];

	end_player_pt = cocos2d::CCPoint((isDirLeft == true) ? start_player_pt.x - skill_iter->charge_dist : start_player_pt.x + skill_iter->charge_dist, start_player_pt.y);

	bool isCameraFixed = SceneManager::Instance()->getIsCameraFixed();
	if(isDirLeft)
	{
		if (end_player_pt.x <= game_obj_info->pos.x)
		{
			if (isSkillFinish)
				isSkillFinish = false;

			if (!isCameraFixed)
				game_obj_info->pos.x -= game_obj_info->move_speed * charge_speed;
		}
		else
			isSkillFinish = true;
	}
	else
	{
		if (end_player_pt.x >= game_obj_info->pos.x)
		{
			if (isSkillFinish)
				isSkillFinish = false;

			if (!isCameraFixed)
				game_obj_info->pos.x += game_obj_info->move_speed * charge_speed;
		}
		else
			isSkillFinish = true;
	}
}
void SkillController::updatePower()
{
	if(ani_info.isAnimationFinish)
	{
		skill_info* curr_skill = getCurrentSkill();

		if(target_obj_info != nullptr)
		{
			if(target_obj_info->health_point > 0)
				target_obj_info->health_point -= game_obj_info->attack_point * curr_skill->power_percentage;
			else
				target_obj_info->health_point = 0;
		}
	}
}
void SkillController::updateChain()
{
	static unsigned int prev_skill_index = 0;

	skill_info* curr_skill = getCurrentSkill();

	auto finded_iter = std::find(curr_skill->chain_hit_in_ani_frame_list.begin(), curr_skill->chain_hit_in_ani_frame_list.end(), ani_info.current_index);

	if(finded_iter != curr_skill->chain_hit_in_ani_frame_list.end())
	{
		if(target_obj_info != nullptr)
		{
			if(*finded_iter == ani_info.current_index)
			{
				if(prev_skill_index != ani_info.current_index)
				{
					if(target_obj_info->health_point > 0)
					{
						target_obj_info->health_point -= game_obj_info->attack_point * curr_skill->power_percentage;
//						BD_CCLog("added = %d", *finded_iter);
						++chain_index;
					}
					else
					{
						target_obj_info->health_point = 0;
						prev_skill_index = 0;
					}

					prev_skill_index = ani_info.current_index;
				}
			}
		}
	}
	else
		prev_skill_index = 0;
}
void SkillController::updateRun()
{
	skill_info* run_skill = getSkill("n_run");

	if (run_skill != nullptr)
	{
		if (onRun)
		{
			unsigned int delta_time = run_end_time - run_start_time;
			unsigned int run_duration = run_skill->power_percentage * 1000;

			if (run_duration < delta_time)
			{
				run_start_time = get_ms_onSystem();
				run_end_time = get_ms_onSystem();

				game_obj_info->move_speed /= 2.f;
				onRun = false;
			}
			else
				run_end_time = get_ms_onSystem();
		}
		else
		{
			run_start_time = get_ms_onSystem();
			run_end_time = get_ms_onSystem();
		}
	}
}
void SkillController::updateHide()
{
	skill_info* hide_skill = getSkill("n_hide");

	if (hide_skill != nullptr)
	{
		if (onHide)
		{
			unsigned int delta_time = hide_end_time - hide_start_time;
			unsigned int hide_duration = hide_skill->power_percentage * 1000;

			unsigned int spr_opacity = g_ctrl->getAlphaValue();
			if (spr_opacity >= (opacity_limit * hide_opacity_limit))
			{
				spr_opacity -= hide_opacity_speed;
				g_ctrl->setAlphaValue(spr_opacity);

				hide_start_time = get_ms_onSystem();
				hide_end_time = get_ms_onSystem();
			}
			else
			{
				if (hide_duration < delta_time)
					onHide = false;
				else
					hide_end_time = get_ms_onSystem();
			}
		}
		else
		{
			if (*game_obj_ev != DEAD)
			{
				unsigned int spr_opacity = g_ctrl->getAlphaValue();
				if (spr_opacity < opacity_limit)
				{
					spr_opacity += hide_opacity_speed;
					if (spr_opacity >= opacity_limit)
						spr_opacity = 255;
				}

				g_ctrl->setAlphaValue(spr_opacity);
			}

			hide_start_time = get_ms_onSystem();
			hide_end_time = get_ms_onSystem();
		}
	}
}