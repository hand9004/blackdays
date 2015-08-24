#include "GameObject.h"
#include "../Utility/Utility.h"
#include "../Graphics/GraphicsController.h"
#include "../Scene/SceneManager.h"
#include "Map.h"
#include <math.h>

const int following_dist = 600;

GameObject::GameObject(void) : target(nullptr), current_event(SEARCHING_RECOGNIZE_AREA), current_index(0),
isSelected(false), isAttackingByEnemy(false), isLeft(false), isNonTargetAttacked(false), 
isCollidedToTarget(false), onDestroy(false)
{
	start_time = get_ms_onSystem();
	end_time = get_ms_onSystem();
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
		case PATROL:
			patrol_update();
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
		if (target != nullptr)
			graphic_ctrl.setTargetPosition(target->getObjectPos());
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
		switch (current_event)
		{
		case MOVE:
		case PATROL:
			start_move_pos.x += delta_x;
			dest_move_pos.x += delta_x;
			patrol_select_pos.x += delta_x;
			enemy_encountered_pos.x += delta_x;
			unsigned int pos_size = move_line_pt.size();
			for (unsigned int i = 0; i < pos_size; ++i)
				move_line_pt.at(i).x += delta_x;
			break;
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

			prev_event = current_event;
			current_event = MOVE;
		}
	}
}
void GameObject::Attack(GameObject* target)
{
	if(!dead_check())
	{
		this->target = target;
		 
		prev_event = current_event;
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
			graphic_ctrl.setActivateComponent(current_skill->ref_ani_name);
		}
	}
}
void GameObject::Patrol(cocos2d::CCPoint patrol_pt)
{
	if (!dead_check())
	{
		if (Map::Instance()->isInMovingArea(patrol_pt))
		{
			start_move_pos = object_info.pos;
			dest_move_pos = patrol_pt;
			patrol_select_pos = dest_move_pos;

			create_move_list(patrol_select_pos);

			graphic_ctrl.setActivateComponent("move");

			prev_event = current_event;
			current_event = PATROL;
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
				/* ī�޶��� ���°� ������ ��� => x,y ��ǥ ��� ����,
				   ī�޶��� ���°� ������� ��� => y ��ǥ�� ���� */
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
	if (!dead_check())
	{
		if (target != nullptr)
		{
			cocos2d::CCPoint target_pos = target->getObjectPos();

			if (check_firing_area(target))
			{
				graphic_ctrl.setActivateComponent("attack");

				float delta_x = object_info.pos.x - target_pos.x;
				if (delta_x <= 0.0f)
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
						if (!object_info.isAreaAttack)
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

				// ���� �ڽ��� �׷��� ��� ���¸� ���������� ��,
				// ������ ���°� ������ �ʾҴٸ� ���� �ִϸ��̼���
				// ���� ������ ��ٸ� ��, ������ ���, �ٽ� ������ �� �ֵ��� �Ѵ�.
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

				// ���°��� ������� ���� ���� - Attack�ȿ��� Ÿ�� ��ü�� ���� �ȿ� �־ �����ؾ��� ���,
				// Ÿ�� ��ü�� ���� �ۿ� �־ �����ϱ� ���� Ÿ�� ��ü�� ���� �������� �ϴ� ���°� �����Ѵ�.
				// Attack ���´� ���� 2���� ��츦 ��� ������ ���̹Ƿ�, ��Ȯ�� ���� ���̽��� ������ �� ����.
				// ��� ���� �ִϸ��̼��� ���¸� ����, ���� �ִϸ��̼��� �������̶��, Ÿ���� ���� �ȿ� ����� ���̹Ƿ�, 
				// ������ ������ ��ü�� �̵��ؼ��� �ȵȴ�. �̵� ���θ� �˷��ִ� �����̴�. �������� ���� ���,
				// �ִϸ��̼��� ��� �ʱ�ȭ�Ǿ� �������� �������
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
void GameObject::patrol_update()
{
	if (!dead_check())
	{
		unsigned int move_pt_size = move_line_pt.size();

		if (current_index < move_pt_size)
		{
			cocos2d::CCPoint move_pos = move_line_pt.at(current_index++);
			object_info.pos = move_pos;

			start_time = get_ms_onSystem();
			end_time = get_ms_onSystem();
		}
		else
		{
			long delta_time = end_time - start_time;

			if (delta_time > object_info.partol_delay_time)
			{
				vector_clear(move_line_pt);

				graphic_ctrl.setActivateComponent("move");
				
				if (patrol_select_pos.x == start_move_pos.x)
					patrol_select_pos = dest_move_pos;
				else
					patrol_select_pos = start_move_pos;

				delta_time = 0;
				current_index = 0;

				start_time = get_ms_onSystem();
				end_time = get_ms_onSystem();

				create_move_list(patrol_select_pos);
			}
			else
			{
				graphic_ctrl.setActivateComponent("stand");

				end_time = get_ms_onSystem();
			}
		}

		search_update();
	}
}
void GameObject::search_update()
{
	if (!dead_check())
	{
		target = nullptr;

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

						if (isLeft)
						{
							recognize_area.setRect(object_info.pos.x - object_info.recognize_area,
								object_info.pos.y - (object_info.recognize_area / 4),
								object_info.recognize_area, object_info.recognize_area / 2);
						}
						else
						{
							recognize_area.setRect(object_info.pos.x, object_info.pos.y - (object_info.recognize_area / 4),
								object_info.recognize_area, object_info.recognize_area / 2);
						}

						if (recognize_area.containsPoint(target_pos))
						{
							auto map_piece_collector = Map::Instance()->getMapPieceList();
							unsigned int map_piece_cnt = map_piece_collector.size();

							bool is_intersect = false;
							for (unsigned int j = 0; j < map_piece_cnt; ++j)
							{
								map_piece* map_piece_iter = map_piece_collector.at(j);

								unsigned int in_map_piece_size = map_piece_iter->in_map_obj_list.size();
								for (unsigned int k = 0; k < in_map_piece_size; ++k)
								{
									in_map_obj* in_map_obj_iter = map_piece_iter->in_map_obj_list.at(k);

									float x = in_map_obj_iter->collision_area.origin.x;
									float y = in_map_obj_iter->collision_area.origin.y;

									float width = in_map_obj_iter->collision_area.getMaxX() - in_map_obj_iter->collision_area.getMinX();
									float height = in_map_obj_iter->collision_area.getMaxY() - in_map_obj_iter->collision_area.getMinY();

									CCPoint rect_vert_pt1 = CCPoint(x + (width / 2), y);
									CCPoint rect_vert_pt2 = CCPoint(x + (width / 2), y + height);

									CCPoint intersect_Y = is_intersect_line_to_line(rect_vert_pt1, rect_vert_pt2, object_info.pos, target_pos);

									bool is_intersect_Y = false;

									is_intersect_Y = in_map_obj_iter->collision_area.containsPoint(intersect_Y);

									if (is_intersect_Y)
									{
										is_intersect = true;
										break;
									}
									else
										is_intersect = false;
								}
							}

							if (is_intersect)
							{
								closest_obj = nullptr;
								BD_CCLog("Can't Finded_Object!");
							}
							else
							{
								if (closest_delta <= 0)
								{
									closest_delta = current_to_other_delta;
									closest_obj = obj_iter;
								}
								else
								{
									if (closest_delta >= current_to_other_delta)
									{
										closest_delta = current_to_other_delta;
										closest_obj = obj_iter;
									}
								}
								BD_CCLog("Finded_Object!");
							}

							//if (closest_delta <= 0)
							//{
							//	closest_delta = current_to_other_delta;
							//	closest_obj = obj_iter;
							//}
							//else
							//{
							//	if (closest_delta >= current_to_other_delta)
							//	{
							//		closest_delta = current_to_other_delta;
							//		closest_obj = obj_iter;
							//	}
							//}
						}
					}
				}
			}
		}

		if(closest_obj != nullptr)
			Attack(closest_obj);
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
			switch (current_event)
			{
			case SKILL:
				current_event = SEARCHING_RECOGNIZE_AREA;
				break;
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
CCPoint GameObject::is_intersect_line_to_line(CCPoint pt1, CCPoint pt2, CCPoint pt3, CCPoint pt4)
{
	bool isintersect = false;
	CCPoint res_pt = CCPoint(0.f, 0.f);

	float A1 = pt2.y - pt1.y;
	float B1 = pt1.x - pt2.x;
	float C1 = A1 * pt1.x + B1 * pt1.y;

	float A2 = pt4.y - pt3.y;
	float B2 = pt3.x - pt4.x;
	float C2 = A2 * pt3.x + B2 * pt3.y;

	float under = A1 * B2 - A2 * B1;

	if (under == 0)
		return res_pt;

	float t = B2 * C1 - B1 * C2;
	float s = A1 * C2 - A2 * C1;

	res_pt.x = t / under;
	res_pt.y = s / under;

	if (res_pt.x >= 0.f	&& res_pt.x <= 1.f &&
		res_pt.y >= 0.f && res_pt.y <= 1.f)
		isintersect = true;
	else
		isintersect = false;

	BD_CCLog("intersect x = %f, y = %f", res_pt.x, res_pt.y);

	return res_pt;

	//double t;

	//double s;

	//double under = (pt4.y - pt3.y)*(pt2.x - pt1.x) - (pt4.x - pt3.x)*(pt2.y - pt1.y);

	//if (under == 0) return false;



	//double _t = (pt4.x - pt3.x)*(pt1.y - pt3.y) - (pt4.y - pt3.y)*(pt1.x - pt3.x);

	//double _s = (pt2.x - pt1.x)*(pt1.y - pt3.y) - (pt2.y - pt1.y)*(pt1.x - pt3.x);



	//t = _t / under;

	//s = _s / under;



	//if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;

	//if (_t == 0 && _s == 0) return false;
}

// �̵��ÿ� ���ȴ�. �̸� ����Ʈ�� �� ����� ���� ����Ʈ�� ���� �̵��Ѵ�.
// �ǽð� ��� �ƴ�.
void GameObject::create_move_list(cocos2d::CCPoint& dest_pt)
{
	if (!move_line_pt.empty())
	{
		current_index = 0;
		vector_clear(move_line_pt);
	}

	float inc_y = (dest_pt.y - object_info.pos.y), inc_x = (dest_pt.x - object_info.pos.x);
	float tan = inc_y / inc_x;

	float atan_val = atan(tan);

	float x_move = 0.0f;
	float y_move = 0.0f;

	// 2 ��и�, 3 ��и鿡 ���� x, y���� ����ó��. ������ ������ ������ ���� 1��и�� 3��и��� ������ ��ġ��,
	// 2��и�� 4 ��и��� ������ ��ĥ �� �ֱ� ������, x, y ���� ���� ������ �ٲ��ش�.
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
// ���ݽÿ� �ַ� ���ȴ�. �ǽð����� ������ ���⸦ ����Ѵ�.
void GameObject::create_move_to_dest_pos(cocos2d::CCPoint& dest_pt)
{
	float inc_y = (dest_pt.y - object_info.pos.y), inc_x = (dest_pt.x - object_info.pos.x);
	float tan = inc_y / inc_x;

	float atan_val = atan(tan);

	float x_move = 0.0f;
	float y_move = 0.0f;

	// 2 ��и�, 3 ��и鿡 ���� x, y���� ����ó��. ������ ������ ������ ���� ���Ⱑ 1��и�� 3��и��� ������ ��ġ��,
	// 2��и�� 4 ��и��� ������ ���Ⱑ ��ĥ �� �ֱ� ������, x, y ���� ���� ������ �ٲ��ش�.
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