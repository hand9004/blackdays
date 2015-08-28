#pragma once
#include "ObjectProtocol.h"

const float charge_speed = 2.0f;

class SkillController
{
public:
	SkillController(void);
	~SkillController(void);
public:
	void Init(obj_info& object_info);
	void Update();
	void Update_UI();
	void Destroy();
public:
	unsigned int getSkillCount() { return game_obj_info->skill_list.size(); }
	skill_info* getCurrentSkill() { return (game_obj_info->skill_list.empty()) ? nullptr : game_obj_info->skill_list.at(current_skill); }
	skill_info* getSkill(const char* skill_type, const char* skill_name)
	{
		skill_info* ret_skill = nullptr;
		unsigned int skill_cnt = game_obj_info->skill_list.size();
		for (unsigned i = 0; i < skill_cnt; ++i)
		{
			skill_info* skill_iter = game_obj_info->skill_list.at(i);
			if (!strcmp(skill_iter->skill_type, skill_type))
			{
				if (!strcmp(skill_iter->skill_name, skill_name))
				{
					ret_skill = skill_iter;
					break;
				}
			}
		}

		return ret_skill;
	}
	bool getIsSkillFinish() { return isSkillFinish; }
	bool getSkillIsTargeting()
	{
		const char* skill_type_name = game_obj_info->skill_list[current_skill]->skill_type;
		bool isSkillTypeTargeting = (!strcmp(skill_type_name, "Charge") || !strcmp(skill_type_name, "Run")
									|| !strcmp(skill_type_name, "Hide") || !strcmp(skill_type_name, "Threat_off")) ? false : true;

		return isSkillTypeTargeting;
	}

	void setObjectSprite(cocos2d::CCSprite* obj_spr) { this->obj_spr = obj_spr; }
	void setSkillSelect(unsigned int skill_sel) { if (skill_sel < skill_cnt) current_skill = skill_sel; }
	void setStartSkillPos(cocos2d::CCPoint start_pt) { start_player_pt = start_pt; }
	void setDirection(bool isLeft) { isDirLeft = isLeft; }
	void setEvent(obj_event& obj_ev) { game_obj_ev = &obj_ev; }
	void setIsSkillFinish(bool isFinish) { isSkillFinish = isFinish; }
	void setAniInfo(grap_to_obj_animate* ani_info) { this->ani_info.current_index = ani_info->current_index;
														this->ani_info.isAnimationFinish = ani_info->isAnimationFinish; }
	void setTargetInfo(obj_info& target) { target_obj_info = &target; }

public:
	void update_Scrolling(float delta_x);
private:
	void updateCharge();
	void updatePower();
	void updateChain();

	void updateRun();
	void updateHide();
	void updateThreatOff();
	void updateSleepTarget();
private:
	grap_to_obj_animate ani_info;
	obj_event* game_obj_ev;
	obj_info* game_obj_info, *target_obj_info;
	bool isDirLeft, isSkillFinish;
	unsigned int current_skill, chain_index, skill_cnt;
	cocos2d::CCPoint start_player_pt, end_player_pt;
	cocos2d::CCSprite* obj_spr;
};

