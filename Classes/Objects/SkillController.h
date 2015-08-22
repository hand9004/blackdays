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
	std::vector<cocos2d::CCSprite*>& getSkillIconList() { return skill_image_icon_list; }
	skill_info* getCurrentSkill() { return game_obj_info->skill_list[current_skill]; }
	bool getIsSkillFinish() { return isSkillFinish; }
	bool getSkillIsTargeting()
	{
		const char* skill_type_name = game_obj_info->skill_list[current_skill]->skill_type;
		bool isSkillTypeTargeting = (!strcmp(skill_type_name, "Charge")) ? false : true;
		return isSkillTypeTargeting;
	}

	void setSkillIconSelect(unsigned int skill_sel) { current_skill = skill_sel; }
	void setIsSelected(bool isSel) { isSelected = isSel; }
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
private:
	grap_to_obj_animate ani_info;
	obj_event* game_obj_ev;
	obj_info* game_obj_info, *target_obj_info;
	bool isSelected, isDirLeft, isSkillFinish;
	unsigned int current_skill, chain_index;
	cocos2d::CCPoint start_player_pt, end_player_pt;
	std::vector<cocos2d::CCSprite*>skill_image_icon_list;
};

