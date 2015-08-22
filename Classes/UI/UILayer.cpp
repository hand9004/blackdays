#include "UILayer.h"
#include "UIController.h"
#include "BD_ActionMove.h"
#include "BD_ActionScrolling.h"

UILayer::UILayer(void) : action_inst(nullptr), after_action_type(NULL), after_action_data(NULL), layer_max_size(0.0f, 0.0f)
{
}

UILayer::~UILayer(void)
{
}
void UILayer::init_UILayer(cocos2d::CCPoint pt)
{
	this->layer_pt = pt;
}
void UILayer::link_to_UILayer(UIComponent* p_ui_comp, cocos2d::CCPoint relative_pt)
{
	CCAssert(p_ui_comp != nullptr, "Can't link to UILayer.");

	cocos2d::CCPoint relative_comp_pt = cocos2d::CCPoint(layer_pt.x + relative_pt.x, layer_pt.y + relative_pt.y);
	p_ui_comp->setPosition(relative_comp_pt);
	
	cocos2d::CCSize ui_size = p_ui_comp->getContentSize();
	float ui_total_size = sqrt(ui_size.width * ui_size.height);
	float current_layer_max_size = sqrt(layer_max_size.width * layer_max_size.height);
	
	if(ui_total_size > current_layer_max_size)
	{
		layer_max_size = ui_size;
		BD_CCLog("layer Size Is = %f %f", layer_max_size.width, layer_max_size.height);
	}

	layer_data* layer_dat = new layer_data;
	layer_dat->relative_pt = relative_pt;
	layer_dat->ui_comp_pt = p_ui_comp;

	layer_comp_list.push_back(layer_dat);
}
void UILayer::link_Action_to_UILayer(const char* action_type, void* action_data)
{
	CCAssert(action_data != nullptr, "Action Data Is NULL.");

	if(!strcmp(action_type, "move_to"))
	{
		action_inst = new BD_ActionMove;
		action_inst->Init(&layer_pt, action_data);
		action_inst->setLayerSize(layer_max_size);
	}
	else if(!strcmp(action_type, "scroll"))
	{
		action_inst = new BD_ActionScrolling;
		action_inst->Init(&layer_pt, action_data);
		action_inst->setLayerSize(layer_max_size);
	}
}
void UILayer::active_Layer(bool isActive)
{
	unsigned int ui_child_size = layer_comp_list.size();
	if(isActive)
	{
		UIController::Instance()->set_TouchEnable_UI(!isActive);

		for(unsigned int i = 0; i < ui_child_size; ++i)
		{
			layer_data* ui_iter = layer_comp_list.at(i);
			ui_iter->ui_comp_pt->set_UIActive(isActive);
			ui_iter->ui_comp_pt->setTouchEnabled(isActive);
		}
	}
	else
	{
		UIController::Instance()->set_TouchEnable_UI(!isActive);

		for(unsigned int i = 0; i < ui_child_size; ++i)
		{
			layer_data* ui_iter = layer_comp_list.at(i);
			ui_iter->ui_comp_pt->set_UIActive(isActive);
			ui_iter->ui_comp_pt->setTouchEnabled(isActive);
		}
	}
}
void UILayer::set_after_layerAction(const char* after_action_type, void* data)
{
	this->after_action_type = after_action_type;
	this->after_action_data = data;
}
void UILayer::update_UILayer()
{
	if(action_inst != nullptr)
	{
		action_inst->Update();

		if(action_inst->getActionEnded())
		{
			if(after_action_type != nullptr)
			{
				if(!strcmp(after_action_type, "Visible"))
				{
					bool isVisible = static_cast<bool>(after_action_data);

					if(isVisible)
						active_Layer(true);
					else
						active_Layer(false);

					after_action_type = nullptr;
				}
			}

			action_inst->Destroy();
			SAFE_DELETE(action_inst);
		}
	}

	// 하위 객체들을 레이어의 움직임에 따라 갱신해준다.
	unsigned int layer_cnt = layer_comp_list.size();
	for(unsigned int i = 0; i < layer_cnt; ++i)
	{
		layer_data* layer_iter = layer_comp_list.at(i);
		cocos2d::CCPoint rel_pt = layer_iter->relative_pt;

		layer_iter->ui_comp_pt->setPosition(layer_pt.x + rel_pt.x, layer_pt.y + rel_pt.y);
	}
}
void UILayer::clearing_UILayer()
{
	if(action_inst != nullptr)
	{
		action_inst->Destroy();
		SAFE_DELETE(action_inst);
	}

	unsigned int layer_size = layer_comp_list.size();
	for(unsigned int i = 0; i < layer_size; ++i)
	{
		layer_data* layer_iter = layer_comp_list.at(i);
		layer_iter->ui_comp_pt = nullptr;
		SAFE_DELETE(layer_iter);
	}

	vector_clear(layer_comp_list);
}
void UILayer::setTouchEvent(TouchEvent t_event, cocos2d::CCPoint delta_pt, cocos2d::CCPoint t_pt)
{
	if(action_inst != nullptr)
		action_inst->setTouchEvent(t_event, delta_pt, t_pt);
}