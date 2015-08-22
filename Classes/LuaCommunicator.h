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
	// Resource 폴더를 기점으로 하여 리소스를 불러온다.
	// Android
	// APK 파일 내부의 Asset 폴더를 기점으로 파일을 버퍼 형식으로 불러온다.
	// apk파일 내의 asset폴더는 따로 접근이 불가함. zip파일 이기 때문.
	const char* getLuaFileChunk(const char *file_name)
	{
		unsigned long file_size = 0;
		char* pt_luachunk = nullptr;
		pt_luachunk = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(file_name, "r", &file_size);

		// 읽고 난 뒤 끝의 더러운 문자들을 없애준다.
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

	// 루아 전역 변수와의 통신을 위해 사용함.
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

	// 루아에 정의된 함수를 불러 C에서 사용하게 해준다.
	void Call_LuaFunction(const char* function_name, const char* sig, ...);
	// C에 정의된 함수를 루아에 등록하여 루아에서 사용하게끔 해준다.
	void Register_CFunction(const char* in_Lua_Variable, lua_CFunction reg_functions);

	lua_State* Lua_GetState();
	void Lua_FileClose();
public:
	int getTableSize(int table_index);
private:
	void lua_Error_msg(const char *fmt, ...);
	lua_State *p_lua_state;
};

