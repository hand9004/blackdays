#pragma once
#include <map>
#include <vector>
#include <cocos2d.h>
#include "UI_Def.h"
#include "UILayer.h"
#include "UIComponent.h"
#include "../LuaCommunicator.h"
#include "../Scene/SceneManager.h"

USING_NS_CC;

class UIController : public cocos2d::CCLayer
{
	UIController(void);
public:
	~UIController(void);
public:
	static UIController* Instance()
	{
		static UIController singleton_instance;
		
		return &singleton_instance;
	}

	virtual bool init()
	{
		if ( !CCLayer::init() )
		{
			return false;
		}

		cocos2d::CCScene* parent_scene = SceneManager::Instance()->getCurrentScene();
		parent_scene->addChild(this, 2);

		schedule(schedule_selector(UIController::update_UIAll));

		return true;
	}

	unsigned int add_UIComponent(UI_TYPE ui_type, const char* key, void* ui_packet);
	unsigned int check_UIComponent();
	void update_UIComponent();
	void clear_UIComponent();

	void add_UILayer(const char* layer_name, cocos2d::CCPoint layer_pt);
	void add_UIComponent_to_Layer(const char* layer_name, unsigned int id, cocos2d::CCPoint relative_pt);
	void add_Action_to_Layer(const char* layer_name, const char* action_type, void* action_data);
	void active_Layer(const char* layer_name, bool isLayerActive);
	void set_after_layerAction(const char* layer_name, const char* after_action_type, void* data);
	void update_UILayer();
	void clear_UILayer();
public:
	void update_UIAll(float dt);
	
	// UI 클래스 내부에서 쓰임. 외부에선 사용 불가.
	UIComponent* find_UIComponent(unsigned int src_id);
	// UI 클래스 외부에서 쓰임. 루아 테이블의 이름을 키로 쓰면 됨.
	UIComponent* find_UIComponent(const char* key);

	void register_UIFunctions();
	unsigned int create_UIFromLua();
	void set_Activate_UI(unsigned int sel_id, bool is_Activate);
	void set_TouchEnable_UI(bool isTouchEnable);
	CREATE_FUNC(UIController);
public:
	bool getIsTouchGrab() { return isGetTouching; }
	void setIsTouchGrab(bool touch_grab) { isGetTouching = touch_grab; }
public:
	void setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
private:
	unsigned int setButton(const char* key);
	unsigned int setCheckBox(const char* key);
	unsigned int setMessageBox(const char* key);
	unsigned int setPopupMessage(const char* key);
	unsigned int setSlideControl(const char* key);
	unsigned int setSlideSelector(const char* key);
	unsigned int setBackground(const char* key);
private:
	std::map<const char*, UILayer*>layer_list;
	std::map<const char*, UIComponent*>ui_component_list;
	unsigned int ui_component_cnt;
	bool isGetTouching;
};