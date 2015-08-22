#include "BD_ActionMove.h"
#include "../Utility/Utility.h"
#include "UILayer.h"

BD_ActionMove::BD_ActionMove(void) : moveSpeed(0.0f), current_pt(nullptr), isVerticalMove(false)
{
}

BD_ActionMove::~BD_ActionMove(void)
{
}

void BD_ActionMove::Init(cocos2d::CCPoint* src_pt, void* action_data)
{
	layer_act_move* move_action_data = static_cast<layer_act_move*>(action_data);

	start_pt = *src_pt;
	current_pt = src_pt;
	moveSpeed = move_action_data->moveSpeed;
	isVerticalMove = move_action_data->isVerticalMove;

	if(isVerticalMove)
	{
		dest_pt.x = current_pt->x;
		dest_pt.y += current_pt->y + move_action_data->delta_move.y;
	}
	else
	{
		dest_pt.x += current_pt->x + move_action_data->delta_move.x;
		dest_pt.y = current_pt->y;
	}
	BD_CCLog("Action_MoveData = %f %f %f %d", dest_pt.x, dest_pt.y, moveSpeed, isVerticalMove);
}
void BD_ActionMove::Update()
{
	if(!isActionEnded)
	{
		if(isVerticalMove)
		{
			float delta_Y = 0.0f;
			if(start_pt.y > dest_pt.y)
			{
				if(dest_pt.y < current_pt->y)
				{
					current_pt->y -= moveSpeed;
					delta_Y = -moveSpeed;
				}
				else
					isActionEnded = true;
			}
			else
			{
				if(dest_pt.y > current_pt->y)
				{
					current_pt->y += moveSpeed;
					delta_Y = moveSpeed;
				}
				else
					isActionEnded = true;
			}
		}
		else
		{
			float delta_X = 0.0f;
			if(start_pt.x > dest_pt.x)
			{
				if(dest_pt.x < current_pt->x)
				{
					current_pt->x -= moveSpeed;
					delta_X = -moveSpeed;
				}
				else
					isActionEnded = true;
			}
			else
			{
				if(dest_pt.x > current_pt->x)
				{
					current_pt->x += moveSpeed;
					delta_X = moveSpeed;
				}
				else
					isActionEnded = true;
			}
		}
	}
}
void BD_ActionMove::Destroy()
{
	current_pt = nullptr;
}
void BD_ActionMove::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt)
{
}