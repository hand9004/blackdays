#include "SkillController.h"
#include "../UI/UIController.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"

#include <algorithm> 

SkillController::SkillController(void) : isSelected(false), isSkillFinish(false), current_skill(0), chain_index(0),
										target_obj_info(nullptr), game_obj_info(nullptr)
{
}

SkillController::~SkillController(void)
{
}

void SkillController::Init(obj_info& object_info)
{
	game_obj_info = &object_info;

	unsigned int skill_size = game_obj_info->skill_list.size();

	for(unsigned int i = 0; i < skill_size; ++i)
	{
		skill_info* skill_iter = game_obj_info->skill_list[i];

		skill_image_icon_list.push_back(cocos2d::CCSprite::create(skill_iter->skill_image_name));

		skill_image_icon_list.at(i)->setVisible(false);

		UIController::Instance()->addChild(skill_image_icon_list.at(i));
	}
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
		else if(!strcmp(skill_type, "Chain"))
			updateChain();
	}

	if(isSkillFinish)
	{
		*game_obj_ev = SEARCHING_RECOGNIZE_AREA;
		chain_index = 0;
	}
}
void SkillController::Update_UI()
{
	unsigned int skill_icon_size = skill_image_icon_list.size();
	for(unsigned int i = 0; i < skill_icon_size; ++i)
	{
		cocos2d::CCSprite* spr_iter = skill_image_icon_list.at(i);

		if(isSelected)
		{
			if(i == current_skill)
				spr_iter->setVisible(true);
			else
				spr_iter->setVisible(false);
		}
		else
			spr_iter->setVisible(false);
	}
}
void SkillController::Destroy()
{
	unsigned int skill_img_size = skill_image_icon_list.size();
	for(unsigned int i = 0; i < skill_img_size; ++i)
		UIController::Instance()->removeChild(skill_image_icon_list.at(i), true);

	vector_clear(skill_image_icon_list);

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