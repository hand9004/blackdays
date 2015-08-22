#pragma once

#include <cocos2d.h>
class SystemInfo
{
	SystemInfo(void);
public:
	~SystemInfo(void);
public:
	static SystemInfo* Instance()
	{
		static SystemInfo* singleton_instance = nullptr;
		if(singleton_instance == nullptr)
			singleton_instance = new SystemInfo;
		
		return singleton_instance;
	}
public:
	void register_SystemFunction();
};

