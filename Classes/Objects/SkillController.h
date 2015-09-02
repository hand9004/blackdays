#pragma once
#include "ObjectProtocol.h"
#include "../Objects/GameGraphicController.h"

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
	bool getIsSkillEmpty() { return game_obj_info->skill_list.empty(); }
	unsigned int getSkillCount() { return (game_obj_info->skill_list.empty()) ? 0 : game_obj_info->skill_list.size(); }
	skill_info* getCurrentSkill() { return (game_obj_info->skill_list.empty()) ? nullptr : game_obj_info->skill_list.at(current_skill); }
	skill_info* getSkill(const char* skill_name)
	{
		skill_info* ret_skill = nullptr;
		if (!game_obj_info->skill_list.empty())
		{
			unsigned int skill_cnt = game_obj_info->skill_list.size();
			for (unsigned i = 0; i < skill_cnt; ++i)
			{
				skill_info* skill_iter = game_obj_info->skill_list.at(i);
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
	bool getOnHide() { return onHide; }
	bool getOnRun() { return onRun; }
	bool getSkillIsTargeting()
	{
		bool isSkillTypeTargeting = false;
		if (!game_obj_info->skill_list.empty())
		{
			const char* skill_type_name = game_obj_info->skill_list.at(current_skill)->skill_type;
			isSkillTypeTargeting = (!strcmp(skill_type_name, "Charge") || !strcmp(skill_type_name, "Run")
				|| !strcmp(skill_type_name, "Hide") || !strcmp(skill_type_name, "Threat_off")) ? false : true;
		}

		return isSkillTypeTargeting;
	}

	void setGraphicController(GameGraphicController* g_ctrl) { this->g_ctrl = g_ctrl; }
	void setObjectSprite(cocos2d::CCSprite* obj_spr) { this->obj_spr = obj_spr; }
	void setStartSkillPos(cocos2d::CCPoint start_pt) { start_player_pt = start_pt; }
	void setDirection(bool isLeft) { isDirLeft = isLeft; }
	void setEvent(obj_event& obj_ev) { game_obj_ev = &obj_ev; }
	void setIsSkillFinish(bool isFinish) { isSkillFinish = isFinish; }
	void setAniInfo(grap_to_obj_animate* ani_info) { this->ani_info.current_index = ani_info->current_index;
														this->ani_info.isAnimationFinish = ani_info->isAnimationFinish; }
	void setTargetInfo(obj_info& target) { target_obj_info = &target; }
	void setOnHide(bool onHide) { this->onHide = onHide; }
	void setOnRun(bool onRun) { this->onRun = onRun; }
	bool setSkillSelect(unsigned int skill_sel)
	{
		if (skill_sel < skill_cnt)
		{
			current_skill = skill_sel;
			return true;
		}
		else
			return false;
	}
	bool setSkillSelect(const char* skill_name)
	{
		if (!game_obj_info->skill_list.empty())
		{
			bool isSetSkill = false;
			unsigned int skill_cnt = game_obj_info->skill_list.size();
			for (unsigned int i = 0; i < skill_cnt; ++i)
			{
				skill_info* skill_iter = game_obj_info->skill_list.at(i);
				if (!strcmp(skill_iter->skill_name, skill_name))
				{
					current_skill = i;
					isSetSkill = true;
					break;
				}
			}

			return isSetSkill;
		}
		else
			return false;
	}
public:
	void update_Scrolling(float delta_x);
private:
	void updateCharge();
	void updatePower();
	void updateChain();

	void updateRun();
	void updateHide();
private:
	GameGraphicController* g_ctrl;
	grap_to_obj_animate ani_info;
	obj_event* game_obj_ev;
	obj_info* game_obj_info, *target_obj_info;
	bool isDirLeft, isSkillFinish, onHide, onRun;
	unsigned int current_skill, chain_index, skill_cnt;
	unsigned long hide_start_time, hide_end_time, run_start_time, run_end_time;
	cocos2d::CCPoint start_player_pt, end_player_pt;
	cocos2d::CCSprite* obj_spr;
};

