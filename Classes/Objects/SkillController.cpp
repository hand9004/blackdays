#include "SkillController.h"
#include "../UI/UIController.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"

#include <algorithm> 

SkillController::SkillController(void) : isSkillFinish(false), current_skill(0), chain_index(0),
										target_obj_info(nullptr), game_obj_info(nullptr), obj_spr(nullptr)
{
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
	unsigned int skill_size = game_obj_info->skill_list.size();
	if(current_skill >= 0 && current_skill < skill_size)
	{
		skill_info* skill_iter = game_obj_info->skill_list[current_skill];

		const char* skill_type = skill_iter->skill_type;
		if(!strcmp(skill_type, "Charge"))
			updateCharge();
		else if(!strcmp(skill_type, "Power"))
			updatePower();
		else if (!strcmp(skill_type, "Chain"))
			updateChain();
		else if (!strcmp(skill_type, "Run"))
			updateRun();
		else if (!strcmp(skill_type, "Hide"))
			updateHide();
		else if (!strcmp(skill_type, "Threat_off"))
			updateThreatOff();
		else if (!strcmp(skill_type, "Sleep_target"))
			updateSleepTarget();
	}

	if(isSkillFinish)
	{
		*game_obj_ev = SEARCHING_RECOGNIZE_AREA;
		chain_index = 0;
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

}
void SkillController::updateHide()
{
}
void SkillController::updateThreatOff()
{

}
void SkillController::updateSleepTarget()
{

}