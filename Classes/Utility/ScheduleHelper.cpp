#include "ScheduleHelper.h"
#include "../UI/UIController.h"

ScheduleHelper::ScheduleHelper(void) : ui_ret(0)
{
	ui_Sche_Func = &ScheduleHelper::ui_scheduler;
	logic_Sche_Func = &ScheduleHelper::logic_scheduler;
}


ScheduleHelper::~ScheduleHelper(void)
{
}

void ScheduleHelper::start_scheduler(SCHEDULER_TYPE sche_type, cocos2d::CCNode* current_ccNode)
{
	switch(sche_type)
	{
	case UI_SCHEDULER:
		current_ccNode->schedule((cocos2d::SEL_SCHEDULE)ui_Sche_Func);
		break;
	case LOGIC_SCHEDULER:
		current_ccNode->schedule((cocos2d::SEL_SCHEDULE)logic_Sche_Func);
		break;
	}
}

void ScheduleHelper::stop_scheduler(SCHEDULER_TYPE sche_type, cocos2d::CCNode* current_ccNode)
{
	switch(sche_type)
	{
	case UI_SCHEDULER:
		current_ccNode->unschedule((cocos2d::SEL_SCHEDULE)ui_Sche_Func);
		break;
	case LOGIC_SCHEDULER:
		current_ccNode->unschedule((cocos2d::SEL_SCHEDULE)logic_Sche_Func);
		break;
	}
}

void ScheduleHelper::ui_scheduler(float dt)
{
	ui_ret = UIController::Instance()->check_UIComponent();
}

void ScheduleHelper::logic_scheduler(float dt)
{
}

unsigned int ScheduleHelper::ui_return()
{
	int ret = ui_ret;
	ui_ret = 0;
	return ret;
}