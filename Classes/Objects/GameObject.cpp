#include "GameObject.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"
#include "Map.h"
#include <math.h>

GameObject::GameObject(void) : target(nullptr), current_event(SEARCHING_RECOGNIZE_AREA), current_index(0),
			isSelected(false), isAttackingByEnemy(false), isLeft(false), isNonTargetAttacked(false), isCollidedToTarget(false), onDestroy(false)
{
}

GameObject::~GameObject(void)
{
}

void GameObject::Init(obj_info* obj_info)
{
	object_info = *obj_info;

	skill_ctrl.Init(object_info);
	hud_ctrl.Init_HUD(object_info);

	graphic_ctrl.add_Object(object_info);
	graphic_ctrl.setPosition(object_info.pos);
}
void GameObject::Update()
{
	if(!onDestroy)
	{
		switch(current_event)
		{
		case NO_EVENT:
			graphic_ctrl.setActivateComponent("stand");
			break;
		case MOVE:
			move_update();
			break;
		case ATTACK:
			attack_update();
			break;
		case SKILL:
			skill_update();
			break;
		case SEARCHING_RECOGNIZE_AREA:
			search_update();
			break;
		case DEAD:
			dead_update();
			break;
		}

		skill_ctrl.setIsSelected(isSelected);
		skill_ctrl.Update_UI();

		check_non_target_attacked();
		check_is_attacked();
		throw_object_update();

		hud_ctrl.isSelected(isSelected);
		hud_ctrl.isAttacked(isAttackingByEnemy);
		hud_ctrl.setObjectImageSize(getObjectRect().size);
		hud_ctrl.Update_HUD();

		graphic_ctrl.setObjectFlipX(isLeft);
		graphic_ctrl.setPosition(object_info.pos);
		graphic_ctrl.update_Object();
	}
}
void GameObject::Destroy()
{
	target = nullptr;

	map_clear(object_info.main_image);

	unsigned int img_frame_size = object_info.img_frame_list.size();
	for(unsigned int i = 0; i < img_frame_size; ++i)
	{
		imgframe_info* img_frame_iter = object_info.img_frame_list[i];
		SAFE_DELETE(img_frame_iter);
	}
	vector_clear(object_info.img_frame_list);

	unsigned int ani_frame_size = object_info.ani_image_list.size();
	for(unsigned int i = 0; i < ani_frame_size; ++i)
	{
		aniframe_info* aniframe_iter = object_info.ani_image_list[i];
		unsigned int in_ani_frame_size = aniframe_iter->in_aniframe_list.size();
		for(unsigned int j = 0; j < in_ani_frame_size; ++j)
		{
			in_aniframe_info* in_aniframe_iter = aniframe_iter->in_aniframe_list[j];
			SAFE_DELETE(in_aniframe_iter);
		}
		vector_clear(aniframe_iter->in_aniframe_list);
		SAFE_DELETE(aniframe_iter);
	}
	vector_clear(object_info.ani_image_list);

	unsigned int ani_frame_set_size = object_info.ani_frame_set_list.size();
	for(unsigned int i = 0; i < ani_frame_set_size; ++i)
	{
		aniframe_set_info* aniframe_set_iter = object_info.ani_frame_set_list[i];
		SAFE_DELETE(aniframe_set_iter);
	}
	vector_clear(object_info.ani_frame_set_list);

	unsigned int effect_size = object_info.effect_list.size();
	for(unsigned int i = 0; i < effect_size; ++i)
	{
		effect_info* effect_iter = object_info.effect_list[i];
		unsigned int effect_set_size = effect_iter->effect_set_list.size();
		for(unsigned int j = 0; j < effect_set_size; ++j)
		{
			effect_set_info* effect_set_iter = effect_iter->effect_set_list[j];
			SAFE_DELETE(effect_set_iter);
		}
		vector_clear(effect_iter->effect_set_list);
		SAFE_DELETE(effect_iter);
	}
	vector_clear(object_info.effect_list);

	unsigned int skill_size = object_info.skill_list.size();
	for(unsigned int i = 0; i < skill_size; ++i)
	{
		skill_info* skill_iter = object_info.skill_list[i];
		vector_clear(skill_iter->chain_hit_in_ani_frame_list);
		SAFE_DELETE(skill_iter);
	}
	vector_clear(object_info.skill_list);

	vector_clear(move_line_pt);
	vector_clear(throwing_object_list);
	vector_clear(all_object_list);

	graphic_ctrl.clear_Object();
	hud_ctrl.Destroy_HUD();
	skill_ctrl.Destroy();
}
void GameObject::change_PosList_On_Scrolling(float delta_x)
{
	if (!dead_check())
	{
		if (current_event == MOVE)
		{
			unsigned int pos_size = move_line_pt.size();
			for (unsigned int i = 0; i < pos_size; ++i)
				move_line_pt.at(i).x += delta_x;
		}
	}
}
void GameObject::change_ThrowPosList_On_Scrolling(float delta_x)
{
	if(!dead_check())
	{
		unsigned int throw_size = throwing_object_list.size();

		for(unsigned int i = 0; i < throw_size; ++i)
		{
			CurveThrowObject* throw_iter = throwing_object_list[i];
			throw_iter->change_PosList_OnScrolling(delta_x);
		}
	}
}
void GameObject::change_SkillPos_On_Scrolling(float delta_x)
{
	if(!dead_check())
		skill_ctrl.update_Scrolling(delta_x);
}
void GameObject::Move(cocos2d::CCPoint dest_pt)
{
	if(!dead_check())
	{
		if(Map::Instance()->isInMovingArea(dest_pt))
		{
			dest_move_pos = dest_pt;

			create_move_list(dest_pt);

			graphic_ctrl.setActivateComponent("move");

			current_event = MOVE;
		}
	}
}
void GameObject::Attack(GameObject* target)
{
	if(!dead_check())
	{
		this->target = target;

		if(!target->dead_check())
			current_event = ATTACK;
	}
}
void GameObject::Skill()
{
	if(!dead_check())
	{
		unsigned int skill_cnt = skill_ctrl.getSkillCount();
		if(skill_cnt > 0)
		{
			skill_info* current_skill = skill_ctrl.getCurrentSkill();
			bool isSkillTargeting = skill_ctrl.getSkillIsTargeting();
			if(isSkillTargeting)
			{
				if(target != nullptr)
				{
					if(!target->dead_check())
					{
						current_event = SKILL;
						skill_ctrl.setDirection(isLeft);
						skill_ctrl.setStartSkillPos(object_info.pos);
						skill_ctrl.setEvent(current_event);
					}
				}
			}
			else
			{
				current_event = SKILL;
				skill_ctrl.setDirection(isLeft);
				skill_ctrl.setStartSkillPos(object_info.pos);
				skill_ctrl.setEvent(current_event);
			}
			graphic_ctrl.setActivateComponent(current_skill->ref_ani_name);
		}
	}
}
void GameObject::SetAllObjectList(std::vector<GameObject*>all_object_list)
{
	if(!dead_check())
		this->all_object_list = all_object_list;
}

void GameObject::move_update()
{
	target = nullptr;

	unsigned int move_pt_size = move_line_pt.size();
	bool isListEmpty = move_line_pt.empty();

	if(!dead_check())
	{
		if(!isListEmpty)
		{
			if (current_index < move_pt_size)
			{
				cocos2d::CCPoint move_pos = move_line_pt.at(current_index++);
				/* 카메라의 상태가 고정일 경우 => x,y 좌표 모두 갱신,
				   카메라의 상태가 비고정일 경우 => y 좌표만 갱신 */
				if (!SceneManager::Instance()->getIsCameraFixed())
					object_info.pos = move_pos;
				else
				{
					if (Map::Instance()->getIsCanScrolling())
						object_info.pos.y = move_pos.y;
					else
						object_info.pos = move_pos;
				}
			}
			else
			{
				current_index = 0;
				dest_move_pos = cocos2d::CCPoint(0.f, 0.f);
				vector_clear(move_line_pt);
				current_event = SEARCHING_RECOGNIZE_AREA;
			}
		}
	}
}
void GameObject::attack_update()
{
	if(!dead_check())
	{
		if(target != nullptr)
		{
			if(check_firing_area(target))
			{
				graphic_ctrl.setActivateComponent("attack");
				cocos2d::CCPoint target_pos = target->getObjectPos();
				float delta_x = target_pos.x - object_info.pos.x;

				if(delta_x >= 0.0f)
					isLeft = false;
				else
					isLeft = true;

				graphic_ctrl.send_Message("attack");
				grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
				grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

				if(ani_packet->isAnimationFinish)
				{
					if(!target->dead_check())
					{
						if(!object_info.isAreaAttack)
							target->decrease_HealthPoint(object_info.attack_point);
						else
						{
							cocos2d::CCRect frame_area = graphic_ctrl.get_Active_Area();
							float max_X = frame_area.getMaxX(), min_X = frame_area.getMinX();
							float fire_X = (isLeft == true) ? min_X : max_X, fire_Y = frame_area.getMidY();
							cocos2d::CCPoint fire_pos = cocos2d::CCPoint(fire_X, fire_Y);

							CurveThrowObject* throw_obj = new CurveThrowObject;
							throw_obj->Init(fire_pos, target->getObjectPos());
							throwing_object_list.push_back(throw_obj);
						}
					}
					else
						current_event = SEARCHING_RECOGNIZE_AREA;
				}

				SAFE_DELETE(ani_packet);
			}
			else
			{
				bool isAttackOver = false;

				// 현재 자신의 그래픽 출력 상태를 따져보았을 때,
				// 공격인 상태가 끝나지 않았다면 공격 애니메이션이
				// 끝날 때까지 기다린 후, 끝났을 경우, 다시 움직일 수 있도록 한다.
				const char* current_ani_stat = graphic_ctrl.getActiveGraphicComp();
				if(!strcmp(current_ani_stat, "attack"))
				{
					graphic_ctrl.send_Message("attack");
					grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
					grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

					if(ani_packet->isAnimationFinish)
						isAttackOver = true;

					SAFE_DELETE(ani_packet);
				}
				else
					isAttackOver = true;

				// 상태값을 사용하지 않은 이유 - Attack안에는 타겟 객체가 범위 안에 있어서 공격해야할 경우,
				// 타겟 객체가 범위 밖에 있어서 공격하기 위해 타겟 객체를 따라 움직여야 하는 상태가 존재한다.
				// Attack 상태는 위의 2가지 경우를 모두 포함한 것이므로, 정확한 세부 케이스는 가려낼 수 없다.
				// 고로 현재 애니메이션의 상태를 보고, 공격 애니메이션을 실행중이라면, 타겟이 범위 안에 들었던 것이므로, 
				// 끝날때 까지는 객체가 이동해서는 안된다. 이동 여부를 알려주는 로직이다. 구분하지 않을 경우,
				// 애니메이션이 계속 초기화되어 움직임이 어색해짐
				if(isAttackOver)
				{
					graphic_ctrl.setActivateComponent("move");
					cocos2d::CCPoint target_pos = target->getObjectPos();
					float delta_x = target_pos.x - object_info.pos.x;
					float target_image_width = target->getObjectRect(target_pos).getMaxX() - target->getObjectRect(target_pos).getMinX();
					float obj_image_width = getObjectRect(object_info.pos).getMaxX() - getObjectRect(object_info.pos).getMinX();

					if (delta_x >= 0.0f)
						target_pos.x -= target_image_width;
					else
						target_pos.x += target_image_width;

					create_move_to_dest_pos(target_pos);

//					BD_CCLog("%s = %f %f %f %f", object_info.object_name, object_info.pos.x, object_info.pos.y, target_pos.x, target_pos.y);
				}
			}
		}
	}
}
void GameObject::skill_update()
{
	if(!dead_check())
	{
		bool isSkillTargeting = skill_ctrl.getSkillIsTargeting();
		if(isSkillTargeting)
		{
			if(target != nullptr)
			{
				if(!target->dead_check())
				{
					if(check_firing_area(target))
					{
						skill_info* curr_skill = skill_ctrl.getCurrentSkill();

						graphic_ctrl.send_Message(curr_skill->ref_ani_name);
						grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
						grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

						skill_ctrl.setAniInfo(ani_packet);
						skill_ctrl.setTargetInfo(target->getObjectInfo());
						skill_ctrl.setIsSkillFinish(ani_packet->isAnimationFinish);

						SAFE_DELETE(ani_packet);
					}
					else
					{
						graphic_ctrl.setActivateComponent("move");
						cocos2d::CCPoint target_pos = target->getObjectPos();
						float delta_x = target_pos.x - object_info.pos.x;
						float target_image_width = target->getObjectRect(target_pos).getMaxX() - target->getObjectRect(target_pos).getMinX();

						if(delta_x >= 0.0f)
							target_pos.x -= target_image_width;
						else
							target_pos.x += target_image_width;

						create_move_to_dest_pos(target_pos);
					}	
				}
				else
				{
					skill_info* curr_skill = skill_ctrl.getCurrentSkill();

					graphic_ctrl.send_Message(curr_skill->ref_ani_name);
					grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
					grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

					skill_ctrl.setAniInfo(ani_packet);
					skill_ctrl.setTargetInfo(target->getObjectInfo());
					skill_ctrl.setIsSkillFinish(ani_packet->isAnimationFinish);

					SAFE_DELETE(ani_packet);
				}
			}
		}
		else
		{
			unsigned int game_obj_size = all_object_list.size();
			cocos2d::CCRect curr_rect = this->getObjectRect();

			for(unsigned int i = 0; i < game_obj_size; ++i)
			{
				GameObject* obj_iter = all_object_list.at(i);

				if(obj_iter != this)
				{
					cocos2d::CCRect obj_rect = obj_iter->getObjectRect();

					if(!obj_iter->dead_check())
					{
						if(!obj_iter->getIsChargeAttacked())
						{
							if(obj_rect.intersectsRect(curr_rect))
							{
								skill_info* curr_skill = skill_ctrl.getCurrentSkill();
								obj_iter->decrease_HealthPoint(object_info.attack_point * curr_skill->power_percentage);
								obj_iter->setIsChargeAttacked(true);
							}
						}
					}
				}
			}
		}
		skill_ctrl.Update();
	}
}
void GameObject::search_update()
{
	if(!dead_check())
	{
		target = nullptr;

		unsigned int object_list_size = all_object_list.size();

		GameObject* closest_obj = nullptr;
		unsigned int closest_delta = 0;
		for(unsigned int i = 0; i < object_list_size; ++i)
		{
			GameObject* obj_iter = all_object_list.at(i);

			if(obj_iter != this)
			{
				if(!obj_iter->dead_check())
				{
					if(obj_iter->getIsEnemy() != object_info.isEnemy)
					{
						cocos2d::CCPoint target_pos = obj_iter->getObjectPos();
						float delta_x = fabs(object_info.pos.x - target_pos.x);
						float delta_y = fabs(object_info.pos.y - target_pos.y);
						float current_to_other_delta = sqrt(delta_x * delta_x) + sqrt(delta_y * delta_y);

						cocos2d::CCRect recognize_area = cocos2d::CCRect(object_info.pos.x - object_info.recognize_area, 
							object_info.pos.y - object_info.recognize_area,
							(object_info.recognize_area * 2), (object_info.recognize_area * 2));

						if(recognize_area.containsPoint(target_pos))
						{
							if(closest_delta <= 0)
							{
								closest_delta = current_to_other_delta;
								closest_obj = obj_iter;
							}
							else
							{
								if(closest_delta >= current_to_other_delta)
								{
									closest_delta = current_to_other_delta;
									closest_obj = obj_iter;
								}
							}
						}
					}
				}
			}
		}

		if(closest_obj != nullptr)
		{
			current_event = ATTACK;
			Attack(closest_obj);
		}

		graphic_ctrl.setActivateComponent("stand");
	}
}
void GameObject::dead_update()
{
	target = nullptr;

	const char* active_component_name = graphic_ctrl.getActiveGraphicComp();

	if(!strcmp(active_component_name, "dead"))
	{
		graphic_ctrl.send_Message("dead");
		grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
		grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

		if(ani_packet->isAnimationFinish)
		{
			unsigned char current_val = graphic_ctrl.getAlphaValue();

			if(current_val > 0)
			{
				current_val -= 5;
				graphic_ctrl.setAlphaValue(current_val);
			}
			else
				onDestroy = true;
		}

		SAFE_DELETE(ani_packet);
	}
	else
	{
		isSelected = isAttackingByEnemy = false;
		graphic_ctrl.setActivateComponent("dead");
	}
}
void GameObject::throw_object_update()
{
	CurveThrowObject* arrived_iter = nullptr;
	unsigned int throw_size = throwing_object_list.size();
	for(unsigned int i = 0; i < throw_size; ++i)
	{
		CurveThrowObject* throw_iter = throwing_object_list.at(i);

		if(throw_iter->isArrived())
			arrived_iter = throw_iter;
		
		throw_iter->Update();
	}

	if(arrived_iter != nullptr)
	{
		unsigned int all_obj_size = all_object_list.size();
		throw_size = throwing_object_list.size();

		if(!arrived_iter->isDestroy())
		{
			if(!arrived_iter->getSendedDamage())
			{
				for(unsigned int i = 0; i < throw_size; ++i)
				{
					CurveThrowObject* throw_iter = throwing_object_list.at(i);
					for(unsigned int j = 0; j < all_obj_size; ++j)
					{
						GameObject* obj_iter = all_object_list.at(j);
						if(obj_iter != this)
						{
							if(!obj_iter->dead_check())
							{
								if(throw_iter->isInBombArea(obj_iter->getObjectPos()))
								{
									if(object_info.isEnemy != obj_iter->getIsEnemy())
									{
										obj_iter->decrease_HealthPoint(object_info.attack_point);
										arrived_iter->setSendedDamage(true);
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			throwing_object_list.erase(std::find(throwing_object_list.begin(), throwing_object_list.end(), arrived_iter));
			arrived_iter->Destroy();
			SAFE_DELETE(arrived_iter);
		}
	}
}
void GameObject::check_non_target_attacked()
{
	if(!dead_check())
	{
		if(skill_ctrl.getIsSkillFinish())
		{
			unsigned int game_obj_size = all_object_list.size();
			for(unsigned int i = 0; i < game_obj_size; ++i)
			{
				GameObject* obj_iter = all_object_list.at(i);

				if(obj_iter != this)
				{
					if(obj_iter->getIsChargeAttacked())
					{
						obj_iter->setIsChargeAttacked(false);
						skill_ctrl.setIsSkillFinish(false);
					}
				}
			}
		}
	}
}
void GameObject::check_is_attacked()
{
	if(!dead_check())
	{
		if(target != nullptr)
		{
			if(!target->dead_check())
			{
				obj_event target_ev = target->getEvent();
				if(target_ev == ATTACK)
					isAttackingByEnemy = true;
				else
					isAttackingByEnemy = false;
			}
			else
				isAttackingByEnemy = false;
		}
		else
			isAttackingByEnemy = false;
	}
}
bool GameObject::dead_check()
{
	if(object_info.health_point <= 0)
	{
		current_event = DEAD;
		object_info.health_point = 0;
		return true;
	}
	else
		return false;
}
bool GameObject::destroy_check()
{
	if(onDestroy)
		return true;
	else
		return false;
}
bool GameObject::check_firing_area(GameObject* target)
{
	cocos2d::CCRect my_Rect = getObjectRect(object_info.pos);
	cocos2d::CCRect firing_area = cocos2d::CCRect((int)(object_info.pos.x - object_info.firing_area_X), (int)(object_info.pos.y - object_info.firing_area_Y), 
		object_info.firing_area_X * 2, object_info.firing_area_Y * 2);

	if (firing_area.containsPoint(target->getObjectPos()))
		isCollidedToTarget = true;
	else
		isCollidedToTarget = false;

	return isCollidedToTarget;
}
// 이동시에 사용된다. 미리 리스트를 쭉 만들어 놓고 리스트에 맞춰 이동한다.
// 실시간 계산 아님.
void GameObject::create_move_list(cocos2d::CCPoint& dest_pt)
{
	if(!move_line_pt.empty())
	{
		current_index = 0;
		vector_clear(move_line_pt);
	}

	float inc_y = (dest_pt.y - object_info.pos.y), inc_x = (dest_pt.x - object_info.pos.x);
	float tan = inc_y / inc_x;

	float atan_val = atan(tan);

	float x_move = 0.0f;
	float y_move = 0.0f;

	// 2 사분면, 3 사분면에 대한 x, y값의 예외처리. 직선의 방정식 상으로 보면 1사분면과 3사분면의 직선이 겹치고,
	// 2사분면과 4 사분면의 직선이 겹칠 수 있기 때문에, x, y 값을 음수 값으로 바꿔준다.
	if(inc_x <= 0.0f && inc_y <= 0.0f
		|| inc_x <= 0.0f && inc_y >= 0.0f)
	{
		x_move = -cos(atan_val);
		y_move = -sin(atan_val);
		isLeft = true;
	}
	else
	{
		x_move = cos(atan_val);
		y_move = sin(atan_val);
		isLeft = false;
	}

	float stand_x = object_info.pos.x, stand_y = object_info.pos.y;

	float delta_x = fabs(inc_x);
	float step = 0.0f;

	while(step < delta_x)
	{
		stand_x += x_move * object_info.move_speed;
		stand_y += y_move * object_info.move_speed;

		step += fabs(x_move * object_info.move_speed);

		move_line_pt.push_back(cocos2d::CCPoint((int)(stand_x), (int)(stand_y)));
	}
}
// 공격시에 주로 사용된다. 실시간으로 대상과의 기울기를 계산한다.
void GameObject::create_move_to_dest_pos(cocos2d::CCPoint& dest_pt)
{
	float inc_y = (dest_pt.y - object_info.pos.y), inc_x = (dest_pt.x - object_info.pos.x);
	float tan = inc_y / inc_x;

	float atan_val = atan(tan);

	float x_move = 0.0f;
	float y_move = 0.0f;

	// 2 사분면, 3 사분면에 대한 x, y값의 예외처리. 직선의 방정식 상으로 보면 기울기가 1사분면과 3사분면의 직선이 겹치고,
	// 2사분면과 4 사분면의 직선의 기울기가 겹칠 수 있기 때문에, x, y 값을 음수 값으로 바꿔준다.
	if(inc_x <= 0.0f && inc_y <= 0.0f
		|| inc_x <= 0.0f && inc_y >= 0.0f)
	{
		x_move = -cos(atan_val);
		y_move = -sin(atan_val);
		isLeft = true;
	}
	else
	{
		x_move = cos(atan_val);
		y_move = sin(atan_val);
		isLeft = false;
	}

	if (SceneManager::Instance()->getIsCameraFixed())
	{
		if (getIsEnemy())
			object_info.pos.x += (int)(x_move * object_info.move_speed);
	}
	else
		object_info.pos.x += (int)(x_move * object_info.move_speed);

	object_info.pos.y += (int)(y_move * object_info.move_speed);
}
void GameObject::skill_to_pos(const char* skill_name, cocos2d::CCPoint& skill_dest_pos)
{
}
void GameObject::skill_to_target(const char* skill_name, GameObject* target)
{
}