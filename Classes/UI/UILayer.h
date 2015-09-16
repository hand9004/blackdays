#pragma once
#include "../Utility/Utility.h"
#include "UIComponent.h"
#include "BD_LayerAction.h"

// 레이어 액션 정보 객체
typedef struct LAYER_ACTION_MOVE
{
	cocos2d::CCPoint delta_move;
	float moveSpeed;
	bool isVerticalMove;
}layer_act_move;

typedef struct LAYER_ACTION_SCROLL
{
	const char* scroll_mode;
	float scrollSpeed;
}layer_act_scroll;

typedef struct LAYER_COLLECTION_DATA
{
	UIComponent* ui_comp_pt;
	cocos2d::CCPoint relative_pt;
}layer_data;

class UILayer
{
public:
	UILayer(void);
	~UILayer(void);
public:
	void init_UILayer(cocos2d::CCPoint pt);
	void link_to_UILayer(UIComponent* p_ui_comp, cocos2d::CCPoint relative_pt);
	void link_Action_to_UILayer(const char* action_type, void* action_data);
	void active_Layer(bool isActive);
	void set_after_layerAction(const char* after_action_type, void* data);
	void update_UILayer();
	void clearing_UILayer();
public:
	void setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt);
public:
	bool is_LayerTouched(cocos2d::CCPoint& touched_Pt)
	{
		bool isTouched = false;
		unsigned int ui_comp_size = layer_comp_list.size();

		for(unsigned int i = 0; i < ui_comp_size; ++i)
		{
			layer_data* ui_iter = layer_comp_list.at(i);

			bool ui_active = ui_iter->ui_comp_pt->get_UIActive();
			cocos2d::CCPoint ui_pos = ui_iter->ui_comp_pt->getPosition();
			cocos2d::CCSize contents_size = ui_iter->ui_comp_pt->getContentSize();
			if(ui_active)
			{
				cocos2d::CCRect contents_Rect = cocos2d::CCRect(ui_pos.x, ui_pos.y, contents_size.width, contents_size.height);

				if(contents_Rect.containsPoint(touched_Pt))
				{
					isTouched = true;
					break;
				}
				else
					isTouched = false;
			}
			else
				isTouched = false;
		}

		return isTouched;
	}
private:
	cocos2d::CCSize layer_max_size;
	cocos2d::CCPoint layer_pt;
	std::vector<layer_data*>layer_comp_list;
	BD_LayerAction* action_inst;
	const char* after_action_type;
	void* after_action_data;
};

