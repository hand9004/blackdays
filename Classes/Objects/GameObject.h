#pragma once
#include "ObjectProtocol.h"
#include "GameGraphicController.h"
#include "HUDCollection.h"
#include "CurveThrowObject.h"
#include "SkillController.h"
#include "../Scene/SceneManager.h"
#include "../Utility/Utility.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
public:
	void Init(obj_info* obj_info);
	void Update();
	void Destroy();
public:
	void change_PosList_On_Scrolling(float delta_x);
	void change_ThrowPosList_On_Scrolling(float delta_x);
	void change_SkillPos_On_Scrolling(float delta_x);
public:
	cocos2d::CCPoint& getObjectPos() { return object_info.pos; }
	cocos2d::CCPoint getDestMovePos() { return dest_move_pos; }
	cocos2d::CCRect getObjectRect() { return graphic_ctrl.get_Active_Area(); }
	cocos2d::CCRect getObjectRect(cocos2d::CCPoint touched_pt) { return graphic_ctrl.get_Touched_Area(touched_pt); }
	unsigned int getRecognizeArea() { return object_info.recognize_area; }
	unsigned int getFiringArea_X() { return object_info.firing_area_X; }
	unsigned int getFiringArea_Y() { return object_info.firing_area_Y; }
	const char* getActiveGraphicComp() { return graphic_ctrl.getActiveGraphicComp(); }
	bool getControllable() { return object_info.isControllable; }
	bool getIsEnemy() { return object_info.isEnemy; }
	bool getIsAttacked() { return isAttackingByEnemy; }
	bool getIsSelected() { return isSelected; }
	bool getIsCollidedToTarget() { return isCollidedToTarget; }
	bool getIsLeft() { return isLeft; }
	bool getIsChargeAttacked() { return isNonTargetAttacked; }
	bool getIsHide() { return onHide; }
	float getMoveSpeed() { return object_info.move_speed; }
	float getMovedDeltaX() { return moved_delta_x; }
	obj_event getEvent(){ return current_event; }
	obj_info& getObjectInfo() { return object_info; }
	GameObject* getTarget() { return target; }

	void increase_HealthPoint(int increasing_pt) { object_info.health_point += increasing_pt; }
	void decrease_HealthPoint(int decreaing_pt) { object_info.health_point -= decreaing_pt; BD_CCLog("HP = %d", object_info.health_point); }
	void addMovedDeltaX(float moved_delta_x) { this->moved_delta_x += moved_delta_x; }
	void setMovedDeltaX(float moved_delta_x) { this->moved_delta_x = moved_delta_x; }
	void setObjectPos(cocos2d::CCPoint dest_pos) { object_info.pos = dest_pos; }
	void setEvent(obj_event obj_ev) { current_event = obj_ev; }
	void setIsChargeAttacked(bool isChargeAttacked) { this->isNonTargetAttacked = isChargeAttacked; }
	void setTarget(GameObject* target) { this->target = target; }
	void setIsSelected(bool isSelected) { this->isSelected = isSelected; }
public:
	void setSkillSelect(unsigned int selected_index) { skill_ctrl.setSkillSelect(selected_index); }

	SkillController* getSkillController() { return &skill_ctrl; }
public:
	void Move(cocos2d::CCPoint dest_pt);
	void Attack(GameObject* target);
	void Skill();
	void Patrol(cocos2d::CCPoint patrol_pt);
	void SetAllObjectList(std::vector<GameObject*>all_object_list);
public:
	bool dead_check();
	bool destroy_check();
private:
	void move_update();
	void attack_update();
	void skill_update();
	void patrol_update();
	void search_update();
	void dead_update();
	void hide_update();

	void throw_object_update();
	bool check_firing_area(GameObject* target);
	bool check_has_map_object_in_line(CCPoint curr_pos, CCPoint target_pos);
	void check_non_target_attacked();
	void check_is_attacked();
private:
	cocos2d::CCPoint is_intersect_line_to_line(CCPoint pt1, CCPoint pt2, CCPoint pt3, CCPoint pt4);

	void create_move_list(cocos2d::CCPoint& dest_pt);
	void create_move_to_dest_pos(cocos2d::CCPoint& dest_pt);

	void skill_to_pos(const char* skill_name, cocos2d::CCPoint& skill_dest_pos);
	void skill_to_target(const char* skill_name, GameObject* target);
private:
	obj_info object_info;
	GameGraphicController graphic_ctrl;
	HUDCollection hud_ctrl;
	SkillController skill_ctrl;
private:
	GameObject* target;
	bool isLeft, isSelected, isAttackingByEnemy, isNonTargetAttacked, isCollidedToTarget, onHide, onDestroy;
	obj_event current_event, prev_event;
	unsigned int current_pos_index;
	std::vector<cocos2d::CCPoint>move_line_pt;
	std::vector<CurveThrowObject*>throwing_object_list;
	std::vector<GameObject*>all_object_list;

	cocos2d::CCPoint dest_move_pos, start_move_pos, *patrol_select_pos;
	long patrol_start_time, patrol_end_time, hide_start_time, hide_end_time;
	float moved_delta_x;
};

