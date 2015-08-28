#pragma once
#include <vector>
#include <cocos2d.h>
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
	void init_Manager();
	void destroy_Manager();

	void init_Stage();
	void update_Stage();
	void destroy_Stage();
public:
	void setStageClear(bool isClear) { isStageClear = isClear; }
	void setVictorious(bool isVictory) { isVictorious = isVictory; }
	void setStageAllClear(bool isAllClear) { isStageAllClear = isAllClear; }
	void setStageIndex(unsigned int index) { stored_stage_index = index; }
	void setStageType(const char* stage_type_str) { stage_type = stage_type_str; }

	const char* getStageType() { return stage_type; }
	unsigned int getStageIndex() { return stored_stage_index; }
	bool getStageUpdateLock() { return isStageUpdateLock; }
private:
	bool isStageStart, isStageClear, isVictorious, isStageAllClear, isStageUpdateLock;
	unsigned int stored_stage_index;
	const char* stage_type;
};