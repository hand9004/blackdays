#pragma once
#include <cocos2d.h>

enum SCHEDULER_TYPE
{
	UI_SCHEDULER,
	LOGIC_SCHEDULER,
};

class ScheduleHelper
{
	ScheduleHelper(void);
public:
	~ScheduleHelper(void);
public:
	static ScheduleHelper* Instance()
	{
		static ScheduleHelper* singleton_instance;
		if(singleton_instance == NULL)
			singleton_instance = new ScheduleHelper;
		
		return singleton_instance;
	}

	void start_scheduler(SCHEDULER_TYPE sche_type, cocos2d::CCNode* current_ccNode);
	void stop_scheduler(SCHEDULER_TYPE sche_type, cocos2d::CCNode* current_ccNode);
	void ui_scheduler(float dt);
	void logic_scheduler(float dt);
	unsigned int ui_return();
private:
	void (ScheduleHelper::*ui_Sche_Func)(float);
	void (ScheduleHelper::*logic_Sche_Func)(float);
	int ui_ret;
};

