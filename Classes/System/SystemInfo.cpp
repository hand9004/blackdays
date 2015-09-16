#include "SystemInfo.h"
#include "../LuaCommunicator.h"

int l_get_screen_size(lua_State* L)
{
	cocos2d::CCSize win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();

	lua_pushinteger(L, win_size.width);
	lua_pushinteger(L, win_size.height);

	return 2;
}

int l_copy_Script_Module_On_Android(lua_State* L)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char src_path[512] = {0,};
	char dest_path[512] = {0,};
	unsigned long file_size = 0;
	std::string writeable_path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();

	const char* folder_name = lua_tostring(L, 1);
	const char* module_name = lua_tostring(L, 2);

	strncpy(src_path, "Script/", sizeof(src_path));
	strncat(src_path, folder_name, sizeof(src_path));
	strncat(src_path, module_name, sizeof(src_path));
	strncat(src_path, ".lua", sizeof(src_path));

	strncpy(dest_path, writeable_path.c_str(), sizeof(dest_path));
	strncat(dest_path, module_name, sizeof(dest_path));
	strncat(dest_path, ".lua", sizeof(dest_path));

	FILE* fp = fopen(dest_path, "r");

	if(fp == nullptr)
	{
		const char* cpy_file = LuaCommunicator::Instance()->getLuaFileChunk(src_path);

		fp = fopen(dest_path, "w");

		if(fp != nullptr)
			fputs((const char*)cpy_file, fp);
		else
			BD_CCLog("File Locate Failed.");

		BD_CCLog("src_path = %s, dest_path = %s", src_path, dest_path);
		BD_CCLog("File = %s", cpy_file);
	}
	else
	{
		fseek(fp, 0L, SEEK_END);
		unsigned int dest_file_size = ftell(fp);
		fclose(fp);

		const char* cpy_file = LuaCommunicator::Instance()->getLuaFileChunk(src_path);
		unsigned int src_file_size = strlen((const char*)cpy_file);

		BD_CCLog("prev_file_size = %d current_file_size = %d", dest_file_size, src_file_size);

		if(dest_file_size != src_file_size)
		{
			fp = fopen(dest_path, "w");

			if(fp != nullptr)
				fputs((const char*)cpy_file, fp);
			else
				BD_CCLog("File Locate Failed.");

			BD_CCLog("File Was Updated.");
			BD_CCLog("src_path = %s, dest_path = %s", src_path, dest_path);
			BD_CCLog("File = %s", cpy_file);
		}
		else
			BD_CCLog("File Is Located Already In Latest Version.");
	}

	fclose(fp);
#else

#endif

	return 0;
}

int l_set_Resource_Path_On_Platform(lua_State* L)
{
	char message[512] = {0,};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string writeable_path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
	strncpy(message, ";", sizeof(message));
	strncat(message, writeable_path.c_str(), sizeof(message));
	strncat(message, "?.lua", sizeof(message));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	std::string file_path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("", "Script/");

	strncpy(message, ";", sizeof(message));
	strncat(message, file_path.c_str(), sizeof(message));
	strncat(message, "?.lua", sizeof(message));
#endif
	BD_CCLog("added_path = %s", message);

	lua_pushstring(L, message);

	return 1;
}

int l_get_platform_type(lua_State* L)
{
	char message[512] = {0,};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strncpy(message, "Android", sizeof(message));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	strncpy(message, "IOS", sizeof(message));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	strncpy(message, "Windows", sizeof(message));
#endif

	BD_CCLog("platform_type = %s", message);
	lua_pushstring(L, message);

	return 1;
}

SystemInfo::SystemInfo(void)
{
}


SystemInfo::~SystemInfo(void)
{
}

void SystemInfo::register_SystemFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("get_screen_size", l_get_screen_size);
	LuaCommunicator::Instance()->Register_CFunction("set_resource_path_on_platform", l_set_Resource_Path_On_Platform);
	LuaCommunicator::Instance()->Register_CFunction("copy_script_module_on_android", l_copy_Script_Module_On_Android);
	LuaCommunicator::Instance()->Register_CFunction("get_platform_type", l_get_platform_type);
}
