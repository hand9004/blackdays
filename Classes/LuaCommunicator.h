#pragma once
#include <stdio.h>
#include <string.h>
#include <tolua++.h>
#include <lua.h>

#include <cocos2d.h>
#include "Utility/Utility.h"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#define SCRIPT_PATH "../Resources/Script/"

class LuaCommunicator
{
	LuaCommunicator(void);
public:
	~LuaCommunicator(void);
public:
	static LuaCommunicator* Instance()
	{
		static LuaCommunicator* singleton_instance = nullptr;
		if(singleton_instance == nullptr)
			singleton_instance = new LuaCommunicator;
		
		return singleton_instance;
	}

	// Windows
	// Resource ������ �������� �Ͽ� ���ҽ��� �ҷ��´�.
	// Android
	// APK ���� ������ Asset ������ �������� ������ ���� �������� �ҷ��´�.
	// apk���� ���� asset������ ���� ������ �Ұ���. zip���� �̱� ����.
	const char* getLuaFileChunk(const char *file_name)
	{
		unsigned long file_size = 0;
		char* pt_luachunk = nullptr;
		pt_luachunk = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(file_name, "r", &file_size);

		// �а� �� �� ���� ������ ���ڵ��� �����ش�.
		if(pt_luachunk != nullptr)
			pt_luachunk[file_size - 1] = '\0';
		else
		{
			BD_CCLog("lua_chunk_load_fail.");
			pt_luachunk = nullptr;
		}

		BD_CCLog("%s", pt_luachunk);
		BD_CCLog("file_size = %ld", file_size);

		cocos2d::CCFileUtils::sharedFileUtils()->purgeCachedEntries();

		return pt_luachunk;
	}
public:
	// Windows O, Android X, iOS ?
	void Lua_FileOpen(const char* file_Name);
	// Windows O, Android O, iOS ?
	void Lua_FileOpenByBuffer(const char* lua_file_chunk);
	void Lua_Register_Functions();

	// ��� ���� �������� ����� ���� �����.
	int Lua_GetGlobalVariable_t_i(const char* table_name, const char* in_table_variable_name);
	const char* Lua_GetGlobalVariable_t_s(const char* table_name, const char* in_table_variable_name);
	float Lua_GetGlobalVariable_t_f(const char* table_name, const char* in_table_variable_name);

	int Lua_GetGlobalVariable_i(const char* variable_name);
	const char* Lua_GetGlobalVariable_s(const char* variable_name);
	float Lua_GetGlobalVariable_f(const char* variable_name);
	void* Lua_GetGlobalVariable_u(const char* variable_name);

	void Lua_SetGlobalVariable_i(const char* variable_name, int data);
	void Lua_SetGlobalVariable_s(const char* variable_name, const char* data);
	void Lua_SetGlobalVariable_f(const char* variable_name, float data);

	// ��ƿ� ���ǵ� �Լ��� �ҷ� C���� ����ϰ� ���ش�.
	void Call_LuaFunction(const char* function_name, const char* sig, ...);
	// C�� ���ǵ� �Լ��� ��ƿ� ����Ͽ� ��ƿ��� ����ϰԲ� ���ش�.
	void Register_CFunction(const char* in_Lua_Variable, lua_CFunction reg_functions);

	lua_State* Lua_GetState();
	void Lua_FileClose();
public:
	int getTableSize(int table_index);
private:
	void lua_Error_msg(const char *fmt, ...);
	lua_State *p_lua_state;
};

