#include "GameObject.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"
#include "../StageManager.h"
#include "Map.h"
#include <math.h>

const unsigned int max_tap_count = 20;
const unsigned int min_tap_count = 10;

GameObject::GameObject(void) : target(nullptr), current_event(SEARCHING_RECOGNIZE_AREA), current_pos_index(0), tap_count(0), objective_tap_count(0),
isSelected(false), isLeft(false), isNonTargetAttacked(false), 
isCollidedToTarget(false), onSleep(false), onDestroy(false)
{
	patrol_start_time = get_ms_onSystem();
	patrol_end_time = get_ms_onSystem();
	patrol_elapsed_time = 0;

	sleep_start_time = get_ms_onSystem();
	sleep_end_time = get_ms_onSystem();
	sleep_elapsed_time = 0;
}

GameObject::~GameObject(void)
{
}

void GameObject::Init(obj_info* obj_info)
{
	object_info = *obj_info;

	set_common_effect();

	skill_ctrl.Init(object_info);

	graphic_ctrl.init();
	graphic_ctrl.add_Object(object_info);
	graphic_ctrl.setPosition(object_info.pos);

	skill_ctrl.setGraphicController(&graphic_ctrl);
	skill_ctrl.setEvent(current_event);

	graphic_ctrl.setActivateComponent("stand");
	CCSprite* curr_spr = graphic_ctrl.getActiveSprite();
	CCSize curr_img_size = curr_spr->getContentSize();

	isSelected = (!strcmp(object_info.object_name, "player")) ? true : false;
	hud_ctrl.isSelected(isSelected);
	hud_ctrl.setObjectImageSize(curr_img_size);
	hud_ctrl.Init_HUD(object_info);
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
			update_move();
			break;
		case ATTACK:
			update_attack();
			break;
		case SKILL:
			update_skill();
			break;
		case PATROL:
			update_patrol();
			break;
		case SEARCHING_RECOGNIZE_AREA:
			update_search();
			break;
		case SLEEP:
			update_sleep();
			break;
		case STUN:
			update_stun();
			break;
		case DEAD:
			update_dead();
			break;
		}

		skill_ctrl.Update();
		skill_ctrl.Update_UI();
		skill_ctrl.setObjectSprite(graphic_ctrl.getActiveSprite());

		check_non_target_attacked();
		update_throw_object();
		update_common_effect();
		
		hud_ctrl.isSelected(isSelected);
		hud_ctrl.setTapCount(tap_count);
		hud_ctrl.setMaxTapCount(objective_tap_count);
		hud_ctrl.setCurrentEvent(current_event);
		hud_ctrl.setOnRun(skill_ctrl.getOnRun());
		hud_ctrl.setOnHide(skill_ctrl.getOnHide());
		hud_ctrl.setObjectImageSize(getObjectRect().size);
		hud_ctrl.Update_HUD();

		graphic_ctrl.setObjectFlipX(isLeft);
		graphic_ctrl.setPosition(object_info.pos);
		if (target != nullptr)
			graphic_ctrl.setTargetPosition(target->getObjectPos());

		graphic_ctrl.update_Object();
	}
}
void GameObject::Destroy()
{
	target = nullptr;

	map_clear(object_info.main_image);

	if (!object_info.img_frame_list.empty())
	{
		unsigned int img_frame_size = object_info.img_frame_list.size();
		for (unsigned int i = 0; i < img_frame_size; ++i)
		{
			imgframe_info* img_frame_iter = object_info.img_frame_list.at(i);
			SAFE_DELETE(img_frame_iter);
		}
		vector_clear(object_info.img_frame_list);
	}

	if (!object_info.ani_image_list.empty())
	{
		unsigned int ani_frame_size = object_info.ani_image_list.size();
		for (unsigned int i = 0; i < ani_frame_size; ++i)
		{
			aniframe_info* aniframe_iter = object_info.ani_image_list.at(i);
			if (!aniframe_iter->in_aniframe_list.empty())
			{
				unsigned int in_ani_frame_size = aniframe_iter->in_aniframe_list.size();
				for (unsigned int j = 0; j < in_ani_frame_size; ++j)
				{
					in_aniframe_info* in_aniframe_iter = aniframe_iter->in_aniframe_list.at(j);
					SAFE_DELETE(in_aniframe_iter);
				}
				vector_clear(aniframe_iter->in_aniframe_list);
				SAFE_DELETE(aniframe_iter);
			}
		}
		vector_clear(object_info.ani_image_list);
	}

	if (!object_info.ani_frame_set_list.empty())
	{
		unsigned int ani_frame_set_size = object_info.ani_frame_set_list.size();
		for (unsigned int i = 0; i < ani_frame_set_size; ++i)
		{
			aniframe_set_info* aniframe_set_iter = object_info.ani_frame_set_list.at(i);
			SAFE_DELETE(aniframe_set_iter);
		}
		vector_clear(object_info.ani_frame_set_list);
	}

	if (!object_info.effect_list.empty())
	{
		unsigned int effect_size = object_info.effect_list.size();
		for (unsigned int i = 0; i < effect_size; ++i)
		{
			effect_info* effect_iter = object_info.effect_list.at(i);
			if (!effect_iter->effect_set_list.empty())
			{
				unsigned int effect_set_size = effect_iter->effect_set_list.size();
				for (unsigned int j = 0; j < effect_set_size; ++j)
				{
					effect_set_info* effect_set_iter = effect_iter->effect_set_list.at(j);
					SAFE_DELETE(effect_set_iter);
				}
				vector_clear(effect_iter->effect_set_list);
				SAFE_DELETE(effect_iter);
			}
		}
		vector_clear(object_info.effect_list);
	}

	if (!object_info.skill_list.empty())
	{
		unsigned int skill_size = object_info.skill_list.size();
		for (unsigned int i = 0; i < skill_size; ++i)
		{
			skill_info* skill_iter = object_info.skill_list.at(i);
			vector_clear(skill_iter->chain_hit_in_ani_frame_list);
			SAFE_DELETE(skill_iter);
		}
		vector_clear(object_info.skill_list);
	}

	vector_clear(move_line_pt);
	vector_clear(throwing_object_list);
	vector_clear(all_object_list);

	graphic_ctrl.clear_Object();
	hud_ctrl.Destroy_HUD();
	skill_ctrl.Destroy();
}
void GameObject::change_PosList_On_Scrolling(int delta_x)
{
	if (!dead_check())
	{
		start_move_pos.x += delta_x;
		dest_move_pos.x += delta_x;
		if (!move_line_pt.empty())
		{
			unsigned int pos_size = move_line_pt.size();
			for (unsigned int i = 0; i < pos_size; ++i)
				move_line_pt.at(i).x += delta_x;
		}

		hud_ctrl.Update_ScrollingPos_HUD(delta_x);
	}
}
void GameObject::change_ThrowPosList_On_Scrolling(int delta_x)
{
	if(!dead_check())
	{
		if (!throwing_object_list.empty())
		{
			unsigned int throw_size = throwing_object_list.size();

			for (unsigned int i = 0; i < throw_size; ++i)
			{
				CurveThrowObject* throw_iter = throwing_object_list.at(i);
				throw_iter->change_PosList_OnScrolling(delta_x);
			}
		}
	}
}
void GameObject::change_SkillPos_On_Scrolling(int delta_x)
{
	if(!dead_check())
		skill_ctrl.update_Scrolling(delta_x);
}
void GameObject::Move(cocos2d::CCPoint dest_pt)
{
	if(!dead_check())
	{
		if (current_event != STUN)
		{
			if (Map::Instance()->isInMovingArea(dest_pt))
			{
				dest_move_pos = dest_pt;

				current_event = MOVE;
				bool is_intersect = check_has_map_object_in_line(object_info.pos, dest_move_pos);

				if (!is_intersect)
				{
					if (skill_ctrl.getOnRun())
						graphic_ctrl.setActivateComponent("run");
					else
						graphic_ctrl.setActivateComponent("move");

					create_move_list(dest_pt);

					hud_ctrl.setMovePoint(dest_pt);

					prev_event = current_event;
				}
				else
				{
					graphic_ctrl.setActivateComponent("stand");
					current_event = SEARCHING_RECOGNIZE_AREA;
				}
			}
		}
	}
}
void GameObject::Attack(GameObject* target)
{
	if(!dead_check())
	{
		if (current_event != STUN)
		{
			this->target = target;

			prev_event = current_event;
			if (!target->dead_check())
				current_event = ATTACK;
		}
	}
}
void GameObject::Skill()
{
	if(!dead_check())
	{
		if (current_event != STUN)
		{
			bool isSkillEmpty = skill_ctrl.getIsSkillEmpty();
			if (!isSkillEmpty)
			{
				skill_info* current_skill = skill_ctrl.getCurrentSkill();

				if (!current_skill->isSkillUsed)
				{
					bool isSkillTargeting = skill_ctrl.getSkillIsTargeting();
					if (isSkillTargeting)
					{
						if (target != nullptr)
						{
							if (!target->dead_check())
							{
								prev_event = current_event;
								current_event = SKILL;
								skill_ctrl.setDirection(isLeft);
								skill_ctrl.setStartSkillPos(object_info.pos);
								skill_ctrl.setEvent(current_event);
							}
						}
					}
					else
					{
						prev_event = current_event;
						current_event = SKILL;
						skill_ctrl.setDirection(isLeft);
						skill_ctrl.setStartSkillPos(object_info.pos);
						skill_ctrl.setEvent(current_event);
					}

					if (strcmp(current_skill->ref_ani_name, "") != 0)
						graphic_ctrl.setActivateComponent(current_skill->ref_ani_name);
				}
			}
		}
	}
}
void GameObject::Patrol(cocos2d::CCPoint patrol_pt, unsigned int patrol_delay_time)
{
	if (!dead_check())
	{
		if (current_event != STUN)
		{
			if (Map::Instance()->isInMovingArea(patrol_pt))
			{
				start_move_pos = object_info.pos;
				dest_move_pos = patrol_pt;
				patrol_select_pos = &dest_move_pos;

				create_move_list(*patrol_select_pos);

				graphic_ctrl.setActivateComponent("move");

				object_info.patrol_delay_time = patrol_delay_time;
				prev_event = current_event;
				current_event = PATROL;
			}
		}
	}
}
void GameObject::SetAllObjectList(std::vector<GameObject*>all_object_list)
{
	if(!dead_check())
		this->all_object_list = all_object_list;
}

void GameObject::update_move()
{
	target = nullptr;

	bool isListEmpty = move_line_pt.empty();

	if(!dead_check())
	{
		if(!isListEmpty)
		{
			unsigned int move_pt_size = move_line_pt.size();
			if (current_pos_index < move_pt_size)
			{
				cocos2d::CCPoint move_pos = move_line_pt.at(current_pos_index++);
				/* 카메라의 상태가 고정일 경우 => x,y 좌표 모두 갱신,
				   카메라의 상태가 비고정일 경우 => y 좌표만 갱신 */
				object_info.pos = move_pos;
			}
			else
			{
				if (StageManager::Instance()->getTalkEvent())
					current_event = NO_EVENT;
				else
					current_event = SEARCHING_RECOGNIZE_AREA;

				current_pos_index = 0;
				dest_move_pos = cocos2d::CCPoint(0.f, 0.f);
				vector_clear(move_line_pt);
				graphic_ctrl.setActivateComponent("stand");
			}
		}
	}
}
void GameObject::update_attack()
{
	if (!dead_check())
	{
		if (target != nullptr)
		{
			if (!target->dead_check())
			{
				if (!target->getSkillController()->getOnHide())
				{
					cocos2d::CCPoint target_pos = target->getObjectPos();

					if (check_firing_area(target))
					{
						graphic_ctrl.setActivateComponent("attack");

						float delta_x = object_info.pos.x - target_pos.x;
						if (delta_x <= 1.0f)
							isLeft = false;
						else
							isLeft = true;

						graphic_ctrl.send_Message("attack");
						grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
						grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

						if (ani_packet->isAnimationFinish)
						{
							if (!target->dead_check())
							{
								Effect* eff_pt = target->get_common_effect("hit_effect");

								if (!object_info.isAreaAttack)
								{
									CCRect target_rect = target->graphic_ctrl.get_Active_Area();

									eff_pt->setObjectPos(target->getObjectInfo().pos);
									eff_pt->setObjectRect(target_rect);
									eff_pt->setEffectRandomSeed();
									eff_pt->setEffectVisible(true);

									target->decrease_HealthPoint(object_info.attack_point);
								}
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
							{
								target = nullptr;

								switch (prev_event)
								{
								case SEARCHING_RECOGNIZE_AREA:
									break;
								case PATROL:
									current_pos_index = 0;
									vector_clear(move_line_pt);
									create_move_list(*patrol_select_pos);

									break;
								}
								current_event = prev_event;
								graphic_ctrl.setActivateComponent("move");
							}
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
						if (!strcmp(current_ani_stat, "attack"))
						{
							graphic_ctrl.send_Message("attack");
							grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
							grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

							if (ani_packet->isAnimationFinish)
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
						if (isAttackOver)
						{
							graphic_ctrl.setActivateComponent("move");
							cocos2d::CCPoint target_pos = target->getObjectPos();
							float delta_x = target_pos.x - object_info.pos.x;
							float target_image_width = target->getObjectRect(target_pos).getMaxX() - target->getObjectRect(target_pos).getMinX();

							if (delta_x >= 0.0f)
								target_pos.x -= target_image_width;
							else
								target_pos.x += target_image_width;

							create_move_to_dest_pos(target_pos);
						}
						else
							graphic_ctrl.setActivateComponent("attack");
					}
				}
				else
				{
					target = nullptr;
					switch (prev_event)
					{
					case SEARCHING_RECOGNIZE_AREA:
						break;
					case PATROL:
						current_pos_index = 0;
						vector_clear(move_line_pt);
						create_move_list(*patrol_select_pos);

						break;
					}
					current_event = prev_event;
					graphic_ctrl.setActivateComponent("move");
				}
			}
			else
			{
				target = nullptr;
				switch (prev_event)
				{
				case SEARCHING_RECOGNIZE_AREA:
					break;
				case PATROL:
					current_pos_index = 0;
					vector_clear(move_line_pt);
					create_move_list(*patrol_select_pos);

					break;
				}
				current_event = prev_event;
				graphic_ctrl.setActivateComponent("move");
			}
		}
	}
}
void GameObject::update_skill()
{
	if (!dead_check())
	{
		if (!skill_ctrl.getIsSkillEmpty())
		{
			bool isSkillTargeting = skill_ctrl.getSkillIsTargeting();
			skill_info* curr_skill = skill_ctrl.getCurrentSkill();
			if (isSkillTargeting)
			{
				if (target != nullptr)
				{
					if (!target->dead_check())
					{
						if (check_firing_area(target))
						{
							graphic_ctrl.send_Message(curr_skill->ref_ani_name);
							grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
							grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

							if (!strcmp(curr_skill->skill_type, "Sleep_target"))
							{
								if (ani_packet->isAnimationFinish)
								{
									target->setEvent(SLEEP);
									target->setTarget(this);
									target->setSleepTimerReset();

									current_event = SEARCHING_RECOGNIZE_AREA;
									curr_skill->isSkillUsed = true;
								}
							}

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

							if (delta_x >= 0.0f)
								target_pos.x -= target_image_width;
							else
								target_pos.x += target_image_width;

							create_move_to_dest_pos(target_pos);
						}
					}
					else
					{
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
				if (!strcmp(curr_skill->skill_type, "Run"))
				{
					if (!skill_ctrl.getOnRun())
					{
						skill_ctrl.setOnRun(true);
						object_info.move_speed *= 2.f;
						current_event = SEARCHING_RECOGNIZE_AREA;
						curr_skill->isSkillUsed = true;
					}
				}
				else if (!strcmp(curr_skill->skill_type, "Hide"))
				{
					graphic_ctrl.send_Message(curr_skill->ref_ani_name);
					grap_to_obj_packet recv_packet = graphic_ctrl.get_Message();
					grap_to_obj_animate* ani_packet = static_cast<grap_to_obj_animate*>(recv_packet.data);

					if (ani_packet->isAnimationFinish)
					{
						skill_ctrl.setOnPrepareHide(true);
						current_event = SEARCHING_RECOGNIZE_AREA;
						graphic_ctrl.setActivateComponent("stand");
					}
					
					skill_ctrl.setIsSkillFinish(ani_packet->isAnimationFinish);
				}
				else if (!strcmp(curr_skill->skill_type, "Charge"))
				{
					if (!all_object_list.empty())
					{
						unsigned int game_obj_size = all_object_list.size();
						cocos2d::CCRect curr_rect = this->getObjectRect();

						for (unsigned int i = 0; i < game_obj_size; ++i)
						{
							GameObject* obj_iter = all_object_list.at(i);

							if (obj_iter != this)
							{
								cocos2d::CCRect obj_rect = obj_iter->getObjectRect();

								if (!obj_iter->dead_check())
								{
									if (!obj_iter->getIsChargeAttacked())
									{
										if (obj_rect.intersectsRect(curr_rect))
										{
											skill_info* curr_skill = skill_ctrl.getCurrentSkill();
											obj_iter->decrease_HealthPoint(object_info.attack_point * curr_skill->power_percentage);
											obj_iter->setIsChargeAttacked(true);
										}
									}
								}
							}
						}

						curr_skill->isSkillUsed = true;
					}
				}
			}
		}
	}
}
void GameObject::update_patrol()
{
	if (!dead_check())
	{
		if (!move_line_pt.empty())
		{
			unsigned int move_pt_size = move_line_pt.size();

			if (current_pos_index < move_pt_size)
			{
				cocos2d::CCPoint move_pos = move_line_pt.at(current_pos_index++);
				object_info.pos = move_pos;

				patrol_start_time = get_ms_onSystem();
				patrol_end_time = get_ms_onSystem();
			}
			else
			{
				long delta_time = patrol_end_time - patrol_start_time;

				if (patrol_elapsed_time > object_info.patrol_delay_time)
				{
					vector_clear(move_line_pt);

					graphic_ctrl.setActivateComponent("move");

					if (patrol_select_pos == &start_move_pos)
						patrol_select_pos = &dest_move_pos;
					else
						patrol_select_pos = &start_move_pos;

					current_pos_index = 0;
					patrol_elapsed_time = 0;

					patrol_start_time = get_ms_onSystem();
					patrol_end_time = get_ms_onSystem();

					create_move_list(*patrol_select_pos);
				}
				else
				{
					graphic_ctrl.setActivateComponent("stand");

					if (delta_time > 0)
					{
						if (patrol_elapsed_time > 0)
							patrol_elapsed_time += delta_time;
						else
							patrol_elapsed_time = delta_time;
					}
					else
						patrol_end_time = get_ms_onSystem();
				}
			}

			update_search();
		}
	}
}
void GameObject::update_search()
{
	if (!dead_check() && !skill_ctrl.getOnHide())
	{
		target = nullptr;

		if (!all_object_list.empty())
		{
			unsigned int object_list_size = all_object_list.size();

			GameObject* closest_obj = nullptr;
			unsigned int closest_delta = 0;
			for (unsigned int i = 0; i < object_list_size; ++i)
			{
				GameObject* obj_iter = all_object_list.at(i);

				if (obj_iter != this)
				{
					if (!obj_iter->dead_check())
					{
						if (obj_iter->getIsEnemy() != object_info.isEnemy)
						{
							cocos2d::CCPoint target_pos = obj_iter->getObjectPos();
							float delta_x = fabs(object_info.pos.x - target_pos.x);
							float delta_y = fabs(object_info.pos.y - target_pos.y);
							float current_to_other_delta = sqrt(delta_x * delta_x + delta_y * delta_y);

							cocos2d::CCRect recognize_area = CCRect(0.f, 0.f, 0.f, 0.f);

							const char* stage_type = StageManager::Instance()->getStageType();
							if (isLeft)
							{
								if (!strcmp(stage_type, "Battle"))
									recognize_area.setRect(object_info.pos.x - object_info.recognize_area,
										object_info.pos.y - (object_info.recognize_area / 2),
										object_info.recognize_area * 2, object_info.recognize_area);
								else
									recognize_area.setRect(object_info.pos.x - object_info.recognize_area,
										object_info.pos.y - (object_info.recognize_area / 2),
										object_info.recognize_area, object_info.recognize_area);
							}
							else
							{
								if (!strcmp(stage_type, "Battle"))
									recognize_area.setRect(object_info.pos.x - object_info.recognize_area, object_info.pos.y - (object_info.recognize_area / 2),
										object_info.recognize_area * 2, object_info.recognize_area);
								else
									recognize_area.setRect(object_info.pos.x, object_info.pos.y - (object_info.recognize_area / 2),
									object_info.recognize_area, object_info.recognize_area);
							}

							if (recognize_area.containsPoint(target_pos))
							{
								bool is_intersect = check_has_map_object_in_line(object_info.pos, target_pos);

								if (is_intersect)
									closest_obj = nullptr;
								else
								{
									// 가장 가까운 객체를 찾아서 먼저 공격한다.
									// 0이하라면, 가장 가까운 객체를 찾지 못한 상태이다.
									// 그러므로 가장 가까운 객체로 첫 객체를 선정한다.
									if (closest_delta <= 0)
									{
										closest_delta = current_to_other_delta;
										closest_obj = obj_iter;
									}
									// 첫 객체가 설정된 이후 가장 가까운 객체를 찾을 때까지 루프를 돈다.
									else
									{
										if (closest_delta >= current_to_other_delta)
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
			}

			if (closest_obj != nullptr)
			{
				if (!closest_obj->getSkillController()->getOnHide())
					Attack(closest_obj);
			}
			else
			{
				switch (current_event)
				{
				case SEARCHING_RECOGNIZE_AREA:
					graphic_ctrl.setActivateComponent("stand");
					break;
				}
			}
		}
	}
}
void GameObject::update_sleep()
{
	if (!dead_check())
	{
		unsigned long delta_time = sleep_end_time - sleep_start_time;
		SkillController* target_skill_ctrl = target->getSkillController();
		skill_info* sleep_skill = target_skill_ctrl->getSkill("n_sleep_target");
		if (sleep_skill != nullptr)
		{
			unsigned int sleep_limit_ms = sleep_skill->power_percentage * 1000;

			if (sleep_elapsed_time > sleep_limit_ms)
			{
				switch (prev_event)
				{
				case PATROL:
					vector_clear(move_line_pt);
					create_move_list(*patrol_select_pos);
					break;
				default:
					prev_event = SEARCHING_RECOGNIZE_AREA;
					break;
				}
				target = nullptr;
				
				current_event = prev_event;

				sleep_elapsed_time = 0;
				sleep_start_time = get_ms_onSystem();
				sleep_end_time = get_ms_onSystem();

				onSleep = false;
			}
			else
			{
				graphic_ctrl.setActivateComponent("dead");

				if (delta_time > 0)
				{
					if (sleep_elapsed_time > 0)
						sleep_elapsed_time += delta_time;
					else
						sleep_elapsed_time = delta_time;
				}
				else
					sleep_end_time = get_ms_onSystem();

				onSleep = true;
			}
		}
	}
}
void GameObject::update_stun()
{
	if (!dead_check())
	{
		// 컨트롤 가능한 객체의 경우
		if (object_info.isControllable)
		{
			if (tap_count > objective_tap_count)
			{
				tap_count = objective_tap_count = 0;
				current_event = SEARCHING_RECOGNIZE_AREA;
				graphic_ctrl.setActivateComponent("stand");
			}
			else
				graphic_ctrl.setActivateComponent("stun");
		}
		// 컨트롤 불가능한 AI 객체의 경우
		// 특정 시간 동안 움직일 수 없음.
		else
		{

		}
	}
}
void GameObject::update_dead()
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
		graphic_ctrl.setActivateComponent("dead");
}
void GameObject::update_throw_object()
{
	CurveThrowObject* arrived_iter = nullptr;

	if (!throwing_object_list.empty())
	{
		unsigned int throw_size = throwing_object_list.size();
		for (unsigned int i = 0; i < throw_size; ++i)
		{
			CurveThrowObject* throw_iter = throwing_object_list.at(i);

			if (throw_iter->isArrived())
				arrived_iter = throw_iter;

			throw_iter->Update();
		}

		if (arrived_iter != nullptr)
		{
			throw_size = throwing_object_list.size();

			if (!all_object_list.empty())
			{
				unsigned int all_obj_size = all_object_list.size();
				if (!arrived_iter->isDestroy())
				{
					if (!arrived_iter->getSendedDamage())
					{
						for (unsigned int i = 0; i < throw_size; ++i)
						{
							CurveThrowObject* throw_iter = throwing_object_list.at(i);
							for (unsigned int j = 0; j < all_obj_size; ++j)
							{
								GameObject* obj_iter = all_object_list.at(j);
								if (obj_iter != this)
								{
									if (!obj_iter->dead_check())
									{
										if (throw_iter->isInBombArea(obj_iter->getObjectPos()))
										{
											if (object_info.isEnemy != obj_iter->getIsEnemy())
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
	}
}
void GameObject::check_non_target_attacked()
{
	if(!dead_check())
	{
		if (skill_ctrl.getIsSkillFinish())
		{
			if (!all_object_list.empty())
			{
				unsigned int game_obj_size = all_object_list.size();
				for (unsigned int i = 0; i < game_obj_size; ++i)
				{
					GameObject* obj_iter = all_object_list.at(i);

					if (obj_iter != this)
					{
						if (obj_iter->getIsChargeAttacked())
						{
							obj_iter->setIsChargeAttacked(false);
							skill_ctrl.setIsSkillFinish(false);
						}
					}
				}
				switch (current_event)
				{
				case SKILL:
					current_event = SEARCHING_RECOGNIZE_AREA;
					break;
				}
			}
		}
	}
}
void GameObject::setStunPreset()
{
	if (!dead_check())
	{
		srand(get_ms_onSystem());

		unsigned int rand_val = max_tap_count - min_tap_count;

		objective_tap_count = min_tap_count;
		objective_tap_count += rand() % rand_val;

		current_event = STUN;
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
bool GameObject::check_has_map_object_in_line(CCPoint curr_pos, CCPoint target_pos)
{
	bool is_intersect = false;
	auto map_piece_collector = Map::Instance()->getMapPieceList();

	if (!map_piece_collector.empty())
	{
		unsigned int map_piece_cnt = map_piece_collector.size();
		for (unsigned int i = 0; i < map_piece_cnt; ++i)
		{
			bool is_intersect_Y_left = false, is_intersect_Y_right = false;
			map_piece* map_piece_iter = map_piece_collector.at(i);

			if (!map_piece_iter->in_map_obj_list.empty())
			{
				unsigned int in_map_piece_size = map_piece_iter->in_map_obj_list.size();
				for (unsigned int j = 0; j < in_map_piece_size; ++j)
				{
					in_map_obj* in_map_obj_iter = map_piece_iter->in_map_obj_list.at(j);

					float x = in_map_obj_iter->obj_spr->getPositionX();
					float y = in_map_obj_iter->obj_spr->getPositionY();

					float width = in_map_obj_iter->collision_area.getMaxX() - in_map_obj_iter->collision_area.getMinX();
					float height = in_map_obj_iter->collision_area.getMaxY() - in_map_obj_iter->collision_area.getMinY();

					CCPoint left_rect_vert_pt1 = CCPoint(x - (width * 0.4), y);
					CCPoint left_rect_vert_pt2 = CCPoint(x - (width * 0.4), y + height);

					CCPoint right_rect_vert_pt1 = CCPoint(x + (width * 0.4), y);
					CCPoint right_rect_vert_pt2 = CCPoint(x + (width * 0.4), y + height);

					CCPoint check_obj_pos = CCPoint(0.f, 0.f);
					CCPoint check_target_pos = CCPoint(0.f, 0.f);

					switch (current_event)
					{
					case MOVE:
						left_rect_vert_pt2 = CCPoint(x - (width * 0.4), y + (height * 0.5));
						right_rect_vert_pt2 = CCPoint(x + (width * 0.4), y + (height * 0.5));

						check_obj_pos = CCPoint(curr_pos.x, curr_pos.y);
						check_target_pos = CCPoint(target_pos.x, target_pos.y);
						break;
					case PATROL:
					case SEARCHING_RECOGNIZE_AREA:
						check_obj_pos = CCPoint(curr_pos.x, curr_pos.y + (height * 0.5));
						check_target_pos = CCPoint(target_pos.x, target_pos.y + (height * 0.5));
						break;
					}

					CCPoint intersect_Y_left = is_intersect_line_to_line(left_rect_vert_pt1, left_rect_vert_pt2, check_obj_pos, check_target_pos);
					CCPoint intersect_Y_right = is_intersect_line_to_line(right_rect_vert_pt1, right_rect_vert_pt2, check_obj_pos, check_target_pos);

					is_intersect_Y_left = in_map_obj_iter->collision_area.containsPoint(intersect_Y_left);
					is_intersect_Y_right = in_map_obj_iter->collision_area.containsPoint(intersect_Y_right);

					if (is_intersect_Y_left || is_intersect_Y_right)
					{
						is_intersect = true;
						break;
					}
					else
						is_intersect = false;
				}
				if (is_intersect)
					break;
			}
		}
	}

	return is_intersect;
}

CCPoint GameObject::is_intersect_line_to_line(CCPoint pt1, CCPoint pt2, CCPoint pt3, CCPoint pt4)
{
	CCPoint res_pt = CCPoint(0.f, 0.f);

	float s1_x, s1_y, s2_x, s2_y;
	s1_x = pt2.x - pt1.x;     s1_y = pt2.y - pt1.y;
	s2_x = pt4.x - pt3.x;     s2_y = pt4.y - pt3.y;

	float s, t;
	s = (-s1_y * (pt1.x - pt3.x) + s1_x * (pt1.y - pt3.y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (pt1.y - pt3.y) - s2_y * (pt1.x - pt3.x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		res_pt.x = pt1.x + (t * s1_x);
		res_pt.y = pt1.y + (t * s1_y);
//		return 1;
	}

//	return 0; // No collision

	return res_pt;
}

// 이동시에 사용된다. 미리 리스트를 쭉 만들어 놓고 리스트에 맞춰 이동한다.
// 실시간 계산 아님.
void GameObject::create_move_list(cocos2d::CCPoint& dest_pt)
{
	if (!move_line_pt.empty())
	{
		current_pos_index = 0;
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

		move_line_pt.push_back(cocos2d::CCPoint(static_cast<int>(stand_x), static_cast<int>(stand_y)));
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

	//if (SceneManager::Instance()->getIsCameraFixed())
	//{
	//	if (getIsEnemy())
	//		object_info.pos.x += static_cast<int>(x_move * object_info.move_speed);
	//	else
	//	{
	//		// 카메라가 고정인 상태에서 같은 아군 AI인 객체는 움직여줘야한다.
	//		if (!getControllable())
	//			object_info.pos.x += static_cast<int>(x_move * object_info.move_speed);
	//	}
	//}
	//else
		object_info.pos.x += static_cast<int>(x_move * object_info.move_speed);

	object_info.pos.y += static_cast<int>(y_move * object_info.move_speed);
}
void GameObject::skill_to_pos(const char* skill_name, cocos2d::CCPoint& skill_dest_pos)
{


}
void GameObject::skill_to_target(const char* skill_name, GameObject* target)
{
}
void GameObject::set_common_effect()
{
	// hit_effect start
	Effect* hit_effect = new Effect;

	effect_info* hit_effect_info = new effect_info;

	hit_effect_info->effect_list_name = "Characters/common_effect.plist";
	hit_effect_info->grap_object_key = "";

	char img_key[256] = { 0, };
	for (unsigned int i = 0; i <= 9; ++i)
	{
		strset(img_key, 0);
		snprintf(img_key, 256, "ef31_00%d.png", i);
		common_eff_str_key.push_back(img_key);
	}

	if (!common_eff_str_key.empty())
	{
		unsigned int key_cnt = common_eff_str_key.size();
		for (unsigned int i = 0; i < key_cnt; ++i)
		{
			effect_set_info* common_eff_set_info = new effect_set_info;

			common_eff_set_info->key = common_eff_str_key.at(i).c_str();
			common_eff_set_info->draw_type = eff_draw_type::DRAW_TO_ME_RANDOM;
			common_eff_set_info->applying_index = 0;
			common_eff_set_info->relative_distance = CCPoint(0.f, 0.f);
		
			hit_effect_info->effect_set_list.push_back(common_eff_set_info);
		}
	}

	hit_effect->Init(hit_effect_info);

	common_effect_list.insert(std::pair<const char*, Effect*>("hit_effect", hit_effect));
	// hit_effect end
}
void GameObject::update_common_effect()
{
	if (!common_effect_list.empty())
	{
		Effect* hit_effect = get_common_effect("hit_effect");

		if (hit_effect != nullptr)
		{
			hit_effect->syncWithEffectTime(30);
			if (hit_effect->isEffectEnd())
				hit_effect->setEffectVisible(false);
		}

		auto begin = common_effect_list.begin();
		auto end = common_effect_list.end();

		for (auto i = begin; i != end; ++i)
		{
			Effect* common_eff_iter = common_effect_list.at(i->first);
			common_eff_iter->Update();
		}
	}
}
Effect* GameObject::get_common_effect(const char* eff_name)
{	
	if (!common_effect_list.empty())
	{
		Effect* ret_eff = nullptr;

		auto begin = common_effect_list.begin();
		auto end = common_effect_list.end();

		for (auto i = begin; i != end; ++i)
		{
			if (!strcmp(i->first, eff_name))
			{
				ret_eff = common_effect_list.at(i->first);
				break;
			}
		}

		return ret_eff;
	}
	else
		return nullptr;
}