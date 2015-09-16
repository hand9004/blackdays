#pragma once
#include <vector>
#include <cocos2d.h>
#include "UI/UIComponent.h"

typedef struct SIMPLE_CONVERSATION_INFO
{
	const char* left_portrait_img_name, *right_portrait_img_name, *message, *restriction;
	cocos2d::ccColor3B message_color;
	unsigned int font_size;
}s_conversation_info;

typedef struct TALK_INFO
{
	const char *message_background;
	std::vector<s_conversation_info*>conversation_list;
}talk_info;

class StageManager
{
	StageManager(void);
public:
	~StageManager(void);
public:
	static StageManager* Instance()
	{
		static StageManager* instance = nullptr;
		if(instance == nullptr)
			instance = new StageManager;

		return instance;
	}
public:
	void register_StageFunction();

	void init_Stage();
	void update_Stage();
	void destroy_Stage();
public:
	void setStageClear(bool isClear) { isStageClear = isClear; }
	void setVictorious(bool isVictory) { isVictorious = isVictory; }
	void setStageAllClear(bool isAllClear) { isStageAllClear = isAllClear; }
	void setTalkEvent(bool isTalkEvent) { this->isTalkEvent = isTalkEvent; }
	void setConversationChange(bool onConversationChange) { this->onConversationChange = onConversationChange; ++current_conversation; }
	void setStageIndex(unsigned int index) { stored_stage_index = index; }
	void setStageType(const char* stage_type_str) { stage_type = stage_type_str; }

	const char* getCurrentTalkName()
	{
		const char* ret_name = nullptr;
		if (current_talk != talk_info_list.end())
		{
			ret_name = current_talk->first;
			return ret_name;
		}
		else
			return ret_name;
	}
	unsigned int getCurrentConversationIndex()	{ return current_conversation; }
	const char* getStageType() { return stage_type; }
	unsigned int getStageIndex() { return stored_stage_index; }
	bool getConversationChange() { return onConversationChange; }
	bool getStageUpdateLock() { return isStageUpdateLock; }
	bool getTalkEvent() { return isTalkEvent; }
	bool getGameRestrict() { return isGameRestrict; }
	bool getUIRestrict() { return isUIRestrict; }
	bool getStageStart() { return isStageStart; }
public:
	void add_talk_event(const char* table_name);
	void active_talk_event(const char* talk_event_name, bool isActive);
private:
	void init_talk_event();
	void update_talk_event();
	void destroy_talk_event();
private:
	bool isStageStart, isStageClear, isVictorious, isStageAllClear, isStageUpdateLock, isTalkEvent, onConversationChange;
	bool isGameRestrict, isUIRestrict;
	unsigned int stored_stage_index, current_conversation;
	const char* stage_type;

	std::map<const char*, talk_info*>talk_info_list;
	std::map<const char*, talk_info*>::iterator current_talk;
	UIComponent* left_img, *right_img, *message_background, *message_font;
};