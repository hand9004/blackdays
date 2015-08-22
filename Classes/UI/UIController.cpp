#include "UIController.h"
#include "BD_Button.h"
#include "BD_CheckBox.h"
#include "BD_Message.h"
#include "BD_PopupMessage.h"
#include "BD_SlideControl.h"
#include "BD_SlideSelector.h"
#include "BD_Background.h"
#include "UILayer.h"
#include "../Utility/Utility.h"
#include "../LuaCommunicator.h"

USING_NS_CC;

#include <algorithm>
// UI 모듈과 루아 스크립트가 통신하기 위한 인터페이스 함수이다.

lua_State* l_state_temp = nullptr;

int l_create_UI(lua_State* L)
{
	l_state_temp = L;
	unsigned int ret_id = UIController::Instance()->create_UIFromLua();

	lua_pushinteger(L, ret_id);

	return 1;
}

int l_update_UI(lua_State* L)
{
	unsigned int ui_ret = UIController::Instance()->check_UIComponent();

	unsigned int ui_id = 0;
	unsigned int ui_sort = 0;
	unsigned int ui_touch = 0;
	unsigned int ui_message = 0;

	if(ui_ret > 0)
	{
		ui_id = (UI_TYPE_FIELD | UI_ID_FIELD) & ui_ret;
		ui_sort = (UI_TYPE_FIELD & ui_ret) >> UI_TYPE_SHIFT;
		ui_touch = (UI_TOUCH_EVENT_FIELD & ui_ret) >> UI_TOUCH_EVENT_SHIFT;
		ui_message = (UI_MESSAGE_FIELD & ui_ret) >> UI_MESSAGE_SHIFT;
	}

	lua_pushinteger(L, ui_id);
	lua_pushinteger(L, ui_sort);
	lua_pushinteger(L, ui_touch);
	lua_pushinteger(L, ui_message);

	BD_CCLog("%d, %d, %d, %d", ui_id, ui_sort, ui_touch, ui_message);

	return 4;
}

int l_set_active_UI(lua_State* L)
{
	bool is_UIActive = false;
	int want_Activate_ID = lua_tointeger(L, 1);
	INTEGER_TO_BOOLEAN(is_UIActive, lua_toboolean(L, 2));

	UIController::Instance()->set_Activate_UI(want_Activate_ID, is_UIActive);

	return 0;
}

int l_create_layer(lua_State* L)
{
	const char* layer_name = lua_tostring(L, 1);
	int layer_pos_X = lua_tointeger(L, 2);
	int layer_pos_Y = lua_tointeger(L, 3);

	UIController::Instance()->add_UILayer(layer_name, cocos2d::CCPoint(layer_pos_X, layer_pos_Y));

	return 0;
}

int l_add_UI_to_layer(lua_State* L)
{
	const char* dest_layer_name = lua_tostring(L, 1);
	unsigned int src_ui_id = lua_tointeger(L, 2);
	int pos_X = lua_tointeger(L, 3);
	int pos_Y = lua_tointeger(L, 4);

	UIController::Instance()->add_UIComponent_to_Layer(dest_layer_name, src_ui_id, cocos2d::CCPoint(pos_X, pos_Y));

	return 0;
}

int l_add_action_to_layer(lua_State* L)
{
	const char* dest_layer_name = lua_tostring(L, 1);
	const char* action_name = lua_tostring(L, 2);

	if(!strcmp(action_name, "move_to"))
	{
		layer_act_move move_action_data;

		int dest_X = 0.0f, dest_Y = 0.0f;
		dest_X = lua_tointeger(L, 3);
		dest_Y = lua_tointeger(L, 4);
		move_action_data.moveSpeed = lua_tonumber(L, 5);
		INTEGER_TO_BOOLEAN(move_action_data.isVerticalMove, lua_toboolean(L, 6));

		move_action_data.delta_move = cocos2d::CCPoint((float)dest_X, (float)dest_Y);
		
		UIController::Instance()->add_Action_to_Layer(dest_layer_name, action_name, (void*)&move_action_data);
	}
	else if(!strcmp(action_name, "scroll"))
	{
		layer_act_scroll scroll_action_data;

		scroll_action_data.scroll_mode = lua_tostring(L, 3);
		scroll_action_data.scrollSpeed = lua_tonumber(L, 4);

		UIController::Instance()->add_Action_to_Layer(dest_layer_name, action_name, (void*)&scroll_action_data);
	}

	return 0;
}

int l_active_layer(lua_State* L)
{
	bool isActiveLayer = false;
	const char* dest_layer_name = lua_tostring(L, 1);
	INTEGER_TO_BOOLEAN(isActiveLayer, lua_toboolean(L, 2));

	UIController::Instance()->active_Layer(dest_layer_name, isActiveLayer);

	return 0;
}

int l_set_after_layerAction(lua_State* L)
{
	const char* dest_layer_name = lua_tostring(L, 1);
	const char* after_action_string = lua_tostring(L, 2);

	if(!strcmp(after_action_string, "Visible"))
	{
		bool isLayerVisible = false;

		INTEGER_TO_BOOLEAN(isLayerVisible, lua_toboolean(L, 3));

		UIController::Instance()->set_after_layerAction(dest_layer_name, after_action_string, (void*)isLayerVisible);

		BD_CCLog("%s %s %d", dest_layer_name, after_action_string, isLayerVisible);
	}

	return 0;
}

int l_send_message(lua_State* L)
{
	const char* message_flag = lua_tostring(L, 1);
	unsigned int dest_id = lua_tointeger(L, 2);

	UIComponent* finded_id = UIController::Instance()->find_UIComponent(dest_id);

	if(!strncmp(message_flag, "POPUP", sizeof(message_flag)))
	{
		pop_msg_update_pack* pop_up_pack = new pop_msg_update_pack;

		strncpy(pop_up_pack->message, lua_tostring(L, 3), sizeof(pop_up_pack->message));
				
		BD_CCLog("id = %d Changed Message = %s", dest_id, pop_up_pack->message);

		finded_id->recv_message_lua(pop_up_pack);
	}
	else if(!strncmp(message_flag, "CHECK_BOX", sizeof(message_flag)))
	{
		bool isChecked = false;
		
		INTEGER_TO_BOOLEAN(isChecked, lua_toboolean(L, 3));

		finded_id->recv_message_lua((void*)isChecked);
	}
	else if(!strncmp(message_flag, "SLIDE_CONTROL", sizeof(message_flag)))
	{
		unsigned int slided_val = lua_tointeger(L, 3);

		finded_id->recv_message_lua((void*)slided_val);
	}

	return 0;
}

// UI 모듈
UIController::UIController(void) : ui_component_cnt(0)
{
	SoundManager::Instance()->preload_EffectMusic("sounds/button_on_click.mp3");
	SoundManager::Instance()->preload_EffectMusic("sounds/check_box_on_click.mp3");
}

UIController::~UIController(void)
{
	SoundManager::Instance()->destroy_EffectMusic("sounds/button_on_click.mp3");
	SoundManager::Instance()->destroy_EffectMusic("sounds/check_box_on_click.mp3");
}

unsigned int UIController::add_UIComponent(UI_TYPE ui_type, const char* key, void* ui_packet)
{
	CCAssert(ui_type >= BUTTON, "UIComponent Type Was Not Defined.");

	UIComponent* ui_data = nullptr;
	switch(ui_type)
	{
	case BUTTON:
		ui_data = new BD_Button();
		break;
	case CHECK_BOX:
		ui_data = new BD_CheckBox();
		break;
	case MESSAGE_BOX:
		ui_data = new BD_Message();
		break;
	case POPUP_MESSAGE:
		ui_data = new BD_PopupMessage();
		break;
	case SLIDE_CONTROL:
		ui_data = new BD_SlideControl();
		break;
	case SLIDE_SELECTOR:
		ui_data = new BD_SlideSelector();
		break;
	case BACKGROUND:
		ui_data = new BD_Background();
		break;
	}

	CCAssert(ui_data != nullptr, "UI Data Wasn't Created.");
	++ui_component_cnt;

	BD_CCLog("%d", ui_component_cnt);

	ui_data->set_ID(ui_component_cnt);
	ui_data->set_Sort(ui_type);

	ui_data->Init(ui_packet);
	ui_component_list[key] = ui_data;

	addChild(ui_data);

	return ui_data->get_ID();
}
unsigned int UIController::check_UIComponent()
{
	bool message_box_active = false;
	unsigned int activate_UI_ID = 0;
	
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();

	for(auto i = begin; i != end; ++i)
	{
		UIComponent* ui_iter = i->second;

		CCAssert(ui_iter != nullptr, "UIComponent Is nullptr");
		bool isTouchEnable = ui_iter->isTouchEnabled();

		// ID 값의 bit-masking을 통하여 현재 UI의 터치 상태가 어떠한가를 알 수 있다.
		// 터치되어 UI에 영향을 준 적이 있다면, 그 UI의 식별자를 리턴한다.
		if(isTouchEnable)
		{
			unsigned int touched = ui_iter->get_UIStatus();
			if(touched > TOUCH_NO_EVENT)
			{
				activate_UI_ID = ui_iter->get_ID();
				ui_iter->set_UIStatus(TOUCH_NO_EVENT);
				BD_CCLog("Event name = %s Event = %d", i->first, touched);
			}
		}
		else
			ui_iter->set_UIStatus(TOUCH_NO_EVENT);
	}

	return activate_UI_ID;
}

void UIController::update_UIComponent()
{
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();

	for(auto i = begin; i != end; ++i)
	{
		UIComponent* ui_iter = i->second;

		ui_iter->Update();
	}
}

void UIController::clear_UIComponent()
{
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();

	for(auto i = begin; i != end; ++i)
	{
		UIComponent* ui_iter = i->second;
		
		ui_iter->Destroy();

		removeChild(ui_iter, true);
	}
	ui_component_cnt = 0;
	map_clear(ui_component_list);
}

void UIController::add_UILayer(const char* layer_name, cocos2d::CCPoint layer_pt)
{
	UILayer* layer_obj = new UILayer;

	CCAssert(layer_obj != nullptr, "Can't Create UILayer.");
	layer_list[layer_name] = layer_obj;
	layer_obj->init_UILayer(layer_pt);
}
void UIController::add_UIComponent_to_Layer(const char* layer_name, unsigned int id, cocos2d::CCPoint relative_pt)
{
	UILayer* p_layer = layer_list[layer_name];

	CCAssert(p_layer != nullptr, "Can't Add To Layer. Should be Make UILayer Object.");
	UIComponent* p_UIComp = find_UIComponent(id);
	p_layer->link_to_UILayer(p_UIComp, relative_pt);
}
void UIController::add_Action_to_Layer(const char* layer_name, const char* action_type, void* action_data)
{
	UILayer* p_layer = layer_list[layer_name];
	p_layer->link_Action_to_UILayer(action_type, action_data);
}
void UIController::active_Layer(const char* layer_name, bool isLayerActive)
{
	UILayer* p_layer = layer_list[layer_name];
	p_layer->active_Layer(isLayerActive);
	BD_CCLog("layer_Active = %s %d", layer_name, isLayerActive);
}
void UIController::set_after_layerAction(const char* layer_name, const char* after_action_type, void* data)
{
	UILayer* p_layer = layer_list[layer_name];
	p_layer->set_after_layerAction(after_action_type, data);
}
void UIController::update_UILayer()
{
	unsigned int layer_cnt = layer_list.size();

	auto iter = layer_list.begin();
	auto begin = layer_list.begin();
	auto end = layer_list.end();

	for(; iter != end; ++iter)
	{
		UILayer* layer_iter = iter->second;
		layer_iter->update_UILayer();
	}
}
void UIController::clear_UILayer()
{
	auto iter = layer_list.begin();
	auto begin = layer_list.begin();
	auto end = layer_list.end();

	for(; iter != end; ++iter)
	{
		UILayer* layer_iter = iter->second;
		layer_iter->clearing_UILayer();
		SAFE_DELETE(layer_iter);
	}

	map_clear(layer_list);
}

void UIController::update_UIAll(float dt)
{
	update_UIComponent();
	update_UILayer();
}

UIComponent* UIController::find_UIComponent(unsigned int src_id)
{
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();
	auto i = begin;

	UIComponent* finded_ui_pt = nullptr;
	for(i = begin; i != end; ++i)
	{
		unsigned int pure_id = i->second->get_PureID();
		if(pure_id == src_id)
		{
			finded_ui_pt = i->second;
			break;
		}
	}

	return finded_ui_pt;
}
UIComponent* UIController::find_UIComponent(const char* key)
{
	UIComponent* ret_pt = nullptr;
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();
	
	for(auto i = begin; i != end; ++i)
	{
		if(!strcmp(i->first, key))
		{
			ret_pt = i->second;
			break;
		}
	}

	return ret_pt;
}
void UIController::register_UIFunctions()
{
	LuaCommunicator::Instance()->Register_CFunction("create_UI", l_create_UI);
	LuaCommunicator::Instance()->Register_CFunction("update_UI", l_update_UI);
	LuaCommunicator::Instance()->Register_CFunction("set_active_UI", l_set_active_UI);

	LuaCommunicator::Instance()->Register_CFunction("create_layer", l_create_layer);
	LuaCommunicator::Instance()->Register_CFunction("add_UI_to_layer", l_add_UI_to_layer);
	LuaCommunicator::Instance()->Register_CFunction("active_layer", l_active_layer);
	LuaCommunicator::Instance()->Register_CFunction("set_after_layerAction", l_set_after_layerAction);
	LuaCommunicator::Instance()->Register_CFunction("add_action_to_layer", l_add_action_to_layer);
	LuaCommunicator::Instance()->Register_CFunction("send_message", l_send_message);
}

unsigned int UIController::create_UIFromLua()
{
	unsigned int ui_type = 0;
	unsigned int ret_id = 0;
	const char* table_name = nullptr;
	table_name = lua_tostring(l_state_temp, 1);
	ui_type = lua_tonumber(l_state_temp, 2);
	lua_pop(l_state_temp, 2);

	UI_TYPE converted_ui_type = (UI_TYPE)ui_type;
	
	lua_getglobal(l_state_temp, table_name);
	CCAssert(lua_istable(l_state_temp, -1), "Error : Wasn't Lua Table Type.");

	switch(converted_ui_type)
	{
	case BUTTON:
		ret_id = UIController::Instance()->setButton(table_name);
		break;
	case CHECK_BOX:
		ret_id = UIController::Instance()->setCheckBox(table_name);
		break;
	case MESSAGE_BOX:
		ret_id = UIController::Instance()->setMessageBox(table_name);
		break;
	case POPUP_MESSAGE:
		ret_id = UIController::Instance()->setPopupMessage(table_name);
		break;
	case SLIDE_CONTROL:
		ret_id = UIController::Instance()->setSlideControl(table_name);
		break;
	case SLIDE_SELECTOR:
		ret_id = UIController::Instance()->setSlideSelector(table_name);
		break;
	case BACKGROUND:
		ret_id = UIController::Instance()->setBackground(table_name);
		break;
	}

	return ret_id;
}

void UIController::set_Activate_UI(unsigned int sel_id, bool is_Activate)
{
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();

	for(auto i = begin; i != end; ++i)
	{
		UIComponent* ui_iter = i->second;

		if(ui_iter->get_PureID() == sel_id)
		{
			ui_iter->set_UIActive(is_Activate);
			break;
		}
	}
}
void UIController::set_TouchEnable_UI(bool isTouchEnable)
{
	auto begin = ui_component_list.begin();
	auto end = ui_component_list.end();

	for(auto i = begin; i != end; ++i)
	{
		UIComponent* ui_iter = i->second;

		ui_iter->setTouchEnabled(isTouchEnable);
	}
}
unsigned int UIController::setButton(const char* key)
{
	int posX, posY;
	butt_pack* butt_info = new butt_pack;

	lua_pushstring(l_state_temp, "untouched");
	lua_gettable(l_state_temp, 1);
	strncpy(butt_info->image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(butt_info->image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "touched");
	lua_gettable(l_state_temp, 1);
	strncpy(butt_info->pressed_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(butt_info->pressed_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	butt_info->image_info.image_pt.x = butt_info->pressed_image_info.image_pt.x = posX;
	butt_info->image_info.image_pt.y = butt_info->pressed_image_info.image_pt.y = posY;	

	return add_UIComponent(BUTTON, key, butt_info);
}
unsigned int UIController::setCheckBox(const char* key)
{
	int posX, posY;
	chk_box_pack* chk_box_info = new chk_box_pack;

	lua_pushstring(l_state_temp, "unchecked");
	lua_gettable(l_state_temp, 1);
	strncpy(chk_box_info->unchecked_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(chk_box_info->unchecked_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "checked");
	lua_gettable(l_state_temp, 1);
	strncpy(chk_box_info->checked_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(chk_box_info->checked_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	chk_box_info->checked_image_info.image_pt.x = chk_box_info->unchecked_image_info.image_pt.x = posX;
	chk_box_info->checked_image_info.image_pt.y = chk_box_info->unchecked_image_info.image_pt.y = posY;	

	return add_UIComponent(CHECK_BOX, key, chk_box_info);
}
unsigned int UIController::setMessageBox(const char* key)
{
	int posX, posY;
	message_pack* msg_box_info = new message_pack;

	lua_pushstring(l_state_temp, "background");
	lua_gettable(l_state_temp, 1);
	strncpy(msg_box_info->background_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(msg_box_info->background_image_info.file_name));
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "message");
	lua_gettable(l_state_temp, 1);
	strncpy(msg_box_info->message, lua_tostring(l_state_temp, -1), sizeof(msg_box_info->message));
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	msg_box_info->background_image_info.image_pt.x = posX;
	msg_box_info->background_image_info.image_pt.y = posY;	

	return add_UIComponent(MESSAGE_BOX, key, msg_box_info);
}
unsigned int UIController::setPopupMessage(const char* key)
{
	int posX, posY;
	pop_message_pack* pop_msg_info = new pop_message_pack;

	lua_pushstring(l_state_temp, "message");
	lua_gettable(l_state_temp, 1);
	strncpy(pop_msg_info->message, lua_tostring(l_state_temp, -1), sizeof(pop_msg_info->message));
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "font_name");
	lua_gettable(l_state_temp, 1);
	strncpy(pop_msg_info->font_name, lua_tostring(l_state_temp, -1), sizeof(pop_msg_info->font_name));
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "mode");
	lua_gettable(l_state_temp, 1);
	strncpy(pop_msg_info->message_mode, lua_tostring(l_state_temp, -1), sizeof(pop_msg_info->message_mode));
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "delay_time_ms");
	lua_gettable(l_state_temp, 1);
	pop_msg_info->delay_time_ms = lua_tonumber(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "font_size");
	lua_gettable(l_state_temp, 1);
	pop_msg_info->font_size = lua_tointeger(l_state_temp, -1);
	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "color");
	lua_gettable(l_state_temp, 1);

	int color_element_size = LuaCommunicator::Instance()->getTableSize(2);
	for(int i = 1; i <= color_element_size; ++i)
	{
		lua_pushinteger(l_state_temp, i);
		lua_gettable(l_state_temp, 2);
	}
	pop_msg_info->color_val.r = lua_tointeger(l_state_temp, -3);
	pop_msg_info->color_val.g = lua_tointeger(l_state_temp, -2);
	pop_msg_info->color_val.b = lua_tointeger(l_state_temp, -1);
	
	lua_pop(l_state_temp, color_element_size);
	lua_pop(l_state_temp, 1);

	pop_msg_info->message_pt.x = posX;
	pop_msg_info->message_pt.y = posY;	
	
	return add_UIComponent(POPUP_MESSAGE, key, pop_msg_info);
}
unsigned int UIController::setSlideControl(const char* key)
{
	int posX, posY;
	slide_ctrl_pack* slide_ctrl_info = new slide_ctrl_pack;

	lua_pushstring(l_state_temp, "slide_background");
	lua_gettable(l_state_temp, 1);
	strncpy(slide_ctrl_info->background_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(slide_ctrl_info->background_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "slide_bar");
	lua_gettable(l_state_temp, 1);
	strncpy(slide_ctrl_info->slide_bar_info.file_name, lua_tostring(l_state_temp, -1), sizeof(slide_ctrl_info->slide_bar_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	slide_ctrl_info->background_image_info.image_pt.x = slide_ctrl_info->slide_bar_info.image_pt.x = posX;
	slide_ctrl_info->background_image_info.image_pt.y = slide_ctrl_info->slide_bar_info.image_pt.y = posY;	

	return add_UIComponent(SLIDE_CONTROL, key, slide_ctrl_info);
}
unsigned int UIController::setSlideSelector(const char* key)
{
	int posX, posY;
	slide_sel_pack* slide_sel_info = new slide_sel_pack;

	lua_pushstring(l_state_temp, "slide_sel_background");
	lua_gettable(l_state_temp, 1);
	strncpy(slide_sel_info->background_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(slide_sel_info->background_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "talk_balloon");
	lua_gettable(l_state_temp, 1);
	strncpy(slide_sel_info->talk_balloon_info.file_name, lua_tostring(l_state_temp, -1), sizeof(slide_sel_info->talk_balloon_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "isVertical");
	lua_gettable(l_state_temp, 1);
	INTEGER_TO_BOOLEAN(slide_sel_info->isVertical, lua_toboolean(l_state_temp, -1));

	lua_pop(l_state_temp, 1);

	slide_sel_info->background_image_info.image_pt.x = posX;
	slide_sel_info->background_image_info.image_pt.y = posY;	

	return add_UIComponent(SLIDE_SELECTOR, key, slide_sel_info);
}
unsigned int UIController::setBackground(const char* key)
{
	int posX, posY;
	bg_pack* bg_info = new bg_pack;

	lua_pushstring(l_state_temp, "background");
	lua_gettable(l_state_temp, 1);
	strncpy(bg_info->background_image_info.file_name, lua_tostring(l_state_temp, -1), sizeof(bg_info->background_image_info.file_name));

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posX");
	lua_gettable(l_state_temp, 1);
	posX = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	lua_pushstring(l_state_temp, "posY");
	lua_gettable(l_state_temp, 1);
	posY = lua_tonumber(l_state_temp, -1);

	lua_pop(l_state_temp, 1);

	bg_info->background_image_info.image_pt.x = posX;
	bg_info->background_image_info.image_pt.y = posY;	

	return add_UIComponent(BACKGROUND, key, bg_info);
}

void UIController::setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	cocos2d::CCPoint touched_pt = pTouch->getLocation();

	auto layer_begin = layer_list.begin();
	auto layer_end = layer_list.end();
	for(auto i = layer_begin; i != layer_end; ++i)
	{
		UILayer* layer_iter = i->second;
		if(layer_iter->is_LayerTouched(touched_pt))
			layer_iter->setTouchEvent(TOUCH_BEGIN, cocos2d::CCPoint(0.0f, 0.0f), touched_pt);
	}

	auto ui_begin = ui_component_list.begin();
	auto ui_end = ui_component_list.end();
	for(auto i = ui_begin; i != ui_end; ++i)
	{
		UIComponent* ui_iter = i->second;
		if(ui_iter->is_UITouched(touched_pt))
		{
			ui_iter->setTouchEvent(TOUCH_BEGIN, touched_pt);
			isGetTouching = true;
		}
	}

	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");
}

void UIController::setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	cocos2d::CCPoint touched_pt = pTouch->getLocation();
	cocos2d::CCPoint prev_pt = pTouch->getPreviousLocation();
	cocos2d::CCPoint delta_pt = cocos2d::CCPoint(touched_pt.x - prev_pt.x, touched_pt.y - prev_pt.y);	

	auto layer_begin = layer_list.begin();
	auto layer_end = layer_list.end();
	for(auto i = layer_begin; i != layer_end; ++i)
	{
		UILayer* layer_iter = i->second;
		if(layer_iter->is_LayerTouched(touched_pt))
			layer_iter->setTouchEvent(TOUCH_MOVED, delta_pt, touched_pt);
	}

	auto ui_begin = ui_component_list.begin();
	auto ui_end = ui_component_list.end();
	for(auto i = ui_begin; i != ui_end; ++i)
	{
		UIComponent* ui_iter = i->second;
		ui_iter->setIsSwipeMode(true);
		if(ui_iter->is_UITouched(touched_pt))
			ui_iter->setTouchEvent(TOUCH_MOVED, touched_pt);
	}

	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");
}

void UIController::setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	cocos2d::CCPoint touched_pt = pTouch->getLocation();
	cocos2d::CCPoint prev_pt = pTouch->getPreviousLocation();
	cocos2d::CCPoint delta_pt = cocos2d::CCPoint(touched_pt.x - prev_pt.x, touched_pt.y - prev_pt.y);	

	auto layer_begin = layer_list.begin();
	auto layer_end = layer_list.end();
	for(auto i = layer_begin; i != layer_end; ++i)
	{
		UILayer* layer_iter = i->second;
		if(layer_iter->is_LayerTouched(touched_pt))
			layer_iter->setTouchEvent(TOUCH_ENDED, delta_pt, touched_pt);
	}

	auto ui_begin = ui_component_list.begin();
	auto ui_end = ui_component_list.end();
	for(auto i = ui_begin; i != ui_end; ++i)
	{
		UIComponent* ui_iter = i->second;

		if(delta_pt.x != 0.0f && delta_pt.y != 0.0f)
			ui_iter->setIsSwipeMode(false);
		else
			ui_iter->setIsSwipeMode(true);

		if(ui_iter->is_UITouched(touched_pt))
			ui_iter->setTouchEvent(TOUCH_ENDED, touched_pt);
	}

	LuaCommunicator::Instance()->Call_LuaFunction("ui_Update", ">");
}
