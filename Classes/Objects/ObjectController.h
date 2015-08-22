#pragma once
#include <vector>
#include <cocos2d.h>
#include "GameObject.h"
#include "../Scene/SceneManager.h"
#include "../UI/UIComponent.h"

USING_NS_CC;

class ObjectController : public cocos2d::CCLayer
{
	ObjectController(void);
public:
	~ObjectController(void);
public:
	static ObjectController* Instance()
	{
		static ObjectController* singleton_instance = nullptr;

		if(singleton_instance == nullptr)
			singleton_instance = new ObjectController;
		 
		return singleton_instance;
	}

	virtual bool init()
	{
		if ( !CCLayer::init())
		{
			return false;
		}

		cocos2d::CCScene* parent_scene = SceneManager::Instance()->getCurrentScene();
		parent_scene->addChild(this, 1);

		return true;
	}

public:
	void add_Object(int obj_x, int obj_y);
	void update_Object();
	void update_Dead_Object();
	void clear_Object();

	void register_ObjectFunction();
public:
	void add_BindUI(UIComponent* src_ui, const char* key);
	void update_BindUI();
public:
	void setTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	void setTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

	void setUpdateScrolling(float delta_x);
public:
	bool getIsObjectDead(const char* object_name);
	unsigned int getEnemyObjectCnt();
	unsigned int getAllyObjectCnt();
public:
	void setLuaState(lua_State* lua_st) { p_lua_st = lua_st; }
private:
	void setObjectInfo(obj_info&);
	void setGraphicFrame(obj_info&);
private:
	void setMainImage(obj_info&);
	void setImageFrame(obj_info&);
	void setAniFrame(obj_info&);
	void setAniFrameSet(obj_info&);
	void setEffect(obj_info&);
	void setSkill(obj_info&);
private:
	void update_Skill_UI(unsigned int index, UIComponent* current_ui);
	void update_Object_UI(unsigned int& index, UIComponent* current_ui);
public:
	CREATE_FUNC(ObjectController);
private:
	GameObject* selected_Object;
	std::map<const char*, UIComponent*>binded_ui_list;
	std::vector<GameObject*>game_object_list;
	bool isScrollingEvent, isTouchSelected;

	lua_State* p_lua_st;
};

