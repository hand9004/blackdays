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
	GameObject* add_Object(int obj_x, int obj_y);
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
	const char* getCollidedTrigger(const char* obj_name);

	GameObject* getObject(unsigned int find_id)
	{
		GameObject* ret_obj = nullptr;
		unsigned int obj_cnt = game_object_list.size();

		for (unsigned int i = 0; i < obj_cnt; ++i)
		{
			GameObject* obj_iter = game_object_list.at(i);

			unsigned int obj_mem_number = (int)obj_iter;
			if (find_id == obj_mem_number)
			{
				ret_obj = obj_iter;
				break;
			}
		}

		return ret_obj;
	}

	GameObject* getObject(const char* obj_name)
	{
		GameObject* ret_obj = nullptr;
		unsigned int obj_cnt = game_object_list.size();

		for (unsigned int i = 0; i < obj_cnt; ++i)
		{
			GameObject* obj_iter = game_object_list.at(i);

			const char* in_obj_name = obj_iter->getObjectInfo().object_name;
			if (!strcmp(obj_name, in_obj_name))
			{
				ret_obj = obj_iter;
				break;
			}
		}

		return ret_obj;
	}
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
public:
	CREATE_FUNC(ObjectController);
private:
	GameObject* selected_Object;
	std::map<const char*, UIComponent*>binded_ui_list;
	std::vector<GameObject*>game_object_list;
	bool isScrollingEvent, isTouchSelected;

	lua_State* p_lua_st;
};

