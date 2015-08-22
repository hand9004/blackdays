#include <cocos2d.h>
#include "LuaCommunicator.h"
#include "Utility\Utility.h"

USING_NS_CC;

int l_DEBUG(lua_State* L)
{
	char message[256] = {0,};
	strncpy(message, lua_tostring(L, 1), sizeof(message));
	BD_CCLog("%s", message);

	return 0;
}

LuaCommunicator::LuaCommunicator(void) : p_lua_state(nullptr)
{
}

LuaCommunicator::~LuaCommunicator(void)
{
}

// Don't Use On Android Platform.
void LuaCommunicator::Lua_FileOpen(const char* file_Name)
{
	char relative_script_path[256] = {0,};
	char log_message[256] = {0,};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strncpy(relative_script_path, cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), sizeof(relative_script_path));
	strncat(relative_script_path, file_Name, sizeof(relative_script_path));

	const char* platform_message = "Activate On Android Platform ";
	strncpy(log_message, platform_message, sizeof(log_message));
	strncat(log_message, relative_script_path, sizeof(log_message));
	BD_CCLog("%s", log_message);
#else
	strncat(relative_script_path, SCRIPT_PATH, sizeof(relative_script_path));
	strncat(relative_script_path, file_Name, sizeof(relative_script_path));
	BD_CCLog("%s", "Activate On Other Platform");
#endif
	p_lua_state = luaL_newstate();
	luaL_openlibs(p_lua_state);

	if(luaL_loadfile(p_lua_state, relative_script_path) || lua_pcall(p_lua_state, 0, 0, 0))
		lua_Error_msg("%s File Can't Open.", relative_script_path);
}
void LuaCommunicator::Lua_FileOpenByBuffer(const char* lua_file_chunk)
{
	p_lua_state = luaL_newstate();
	luaL_openlibs(p_lua_state);
	
	int err = luaL_loadbuffer(p_lua_state, (char*)lua_file_chunk, strlen((char*)lua_file_chunk), "line") ||
			lua_pcall(p_lua_state, 0, 0, 0);

	if(err)
		lua_Error_msg("Error : %s", lua_tostring(p_lua_state, -1));
}

void LuaCommunicator::Lua_Register_Functions()
{
	Register_CFunction("LOG", l_DEBUG);
}

int LuaCommunicator::Lua_GetGlobalVariable_t_i(const char* table_name, const char* in_table_variable_name)
{
	int ret = 0;
	lua_getglobal(p_lua_state, table_name);
	if(!lua_istable(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not table.", table_name);
		return 0;
	}

	lua_getfield(p_lua_state, -1, in_table_variable_name);
	ret = lua_tointeger(p_lua_state, -1);
	if(!lua_isnumber(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Number.", in_table_variable_name);
		return 0;
	}
	lua_pop(p_lua_state, 1);

	return ret;
}
const char* LuaCommunicator::Lua_GetGlobalVariable_t_s(const char* table_name, const char* in_table_variable_name)
{
	const char* ret = nullptr;
	lua_getglobal(p_lua_state, table_name);
	if(!lua_istable(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not table.", table_name);
		return 0;
	}

	lua_getfield(p_lua_state, -1, in_table_variable_name);
	ret = lua_tolstring(p_lua_state, -1, nullptr);
	if(!lua_isstring(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Number.", in_table_variable_name);
		return 0;
	}
	lua_pop(p_lua_state, 1);

	return ret;
}
float LuaCommunicator::Lua_GetGlobalVariable_t_f(const char* table_name, const char* in_table_variable_name)
{
	float ret = 0;
	lua_getglobal(p_lua_state, table_name);
	if(!lua_istable(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not table.", table_name);
		return 0;
	}

	lua_getfield(p_lua_state, -1, in_table_variable_name);
	ret = lua_tonumber(p_lua_state, -1);
	if(!lua_isnumber(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Number.", in_table_variable_name);
		return 0;
	}
	lua_pop(p_lua_state, 1);

	return ret;
}
int LuaCommunicator::Lua_GetGlobalVariable_i(const char* variable_name)
{
	int ret = 0;
	lua_getglobal(p_lua_state, variable_name);
	if(!lua_isnumber(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Number.", variable_name);
		return 0;
	}

	ret = lua_tointeger(p_lua_state, -1);
	lua_pop(p_lua_state, 1);
	return ret;
}
const char* LuaCommunicator::Lua_GetGlobalVariable_s(const char* variable_name)
{
	const char* ret = nullptr;
	lua_getglobal(p_lua_state, variable_name);
	if(!lua_isstring(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not String.", variable_name);
		return 0;
	}

	ret = lua_tolstring(p_lua_state, -1, nullptr);
	lua_pop(p_lua_state, 1);
	return ret;
}
float LuaCommunicator::Lua_GetGlobalVariable_f(const char* variable_name)
{
	float ret = 0.0f;
	lua_getglobal(p_lua_state, variable_name);
	if(!lua_isnumber(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Number.", variable_name);
		return 0;
	}

	ret = lua_tonumber(p_lua_state, -1);
	lua_pop(p_lua_state, 1);
	return ret;
}
void* LuaCommunicator::Lua_GetGlobalVariable_u(const char* variable_name)
{
	void* ret = nullptr;
	lua_getglobal(p_lua_state, variable_name);
	if(!lua_isuserdata(p_lua_state, -1))
	{
		lua_Error_msg("%s Is not Userdata.", variable_name);
		return 0;
	}

	ret = lua_touserdata(p_lua_state, -1);
	lua_pop(p_lua_state, 1);
	return ret;
}

void LuaCommunicator::Lua_SetGlobalVariable_i(const char* variable_name, int data)
{
	float ret = 0.0f;
	lua_setglobal(p_lua_state, variable_name);
	lua_pushinteger(p_lua_state, data);

	lua_pop(p_lua_state, 1);
}
void LuaCommunicator::Lua_SetGlobalVariable_s(const char* variable_name, const char* data)
{
	float ret = 0.0f;
	lua_setglobal(p_lua_state, variable_name);
	lua_pushstring(p_lua_state, data);

	lua_pop(p_lua_state, 1);
}
void LuaCommunicator::Lua_SetGlobalVariable_f(const char* variable_name, float data)
{
	float ret = 0.0f;
	lua_setglobal(p_lua_state, variable_name);
	lua_pushnumber(p_lua_state, data);

	lua_pop(p_lua_state, 1);
}
void LuaCommunicator::Call_LuaFunction(const char* function_name, const char* sig, ...)
{
	va_list vl;
	int narg = 0, nres = 0;

	va_start(vl, sig);
	lua_getglobal(p_lua_state, function_name);

	for(narg = 0; *sig != '>'; ++narg)
	{
		luaL_checkstack(p_lua_state, 1, "too many arguments");

		switch(*sig)
		{
		case 'f':
			lua_pushnumber(p_lua_state, va_arg(vl, double));
			break;
		case 'i':
			lua_pushinteger(p_lua_state, va_arg(vl, int));
			break;
		case 's':
			lua_pushstring(p_lua_state, va_arg(vl, char*));
			break;
		default:
			break;
		}
		++sig;
	}
	++sig;
	nres = strlen(sig);

	if(lua_pcall(p_lua_state, narg, nres, 0) != 0)
		lua_Error_msg("error calling '%s' : %s", function_name, lua_tostring(p_lua_state, -1));

	nres = -nres;
	for(; (*sig != '\0'); ++sig)
	{
		switch(*sig)
		{
		case 'f':
			if(!lua_isnumber(p_lua_state, nres))
				lua_Error_msg("wrong type.");
			*va_arg(vl, double*) = lua_tonumber(p_lua_state, nres);
			break;
		case 'i':
			if(!lua_isnumber(p_lua_state, nres))
				lua_Error_msg("wrong type.");
			*va_arg(vl, int*) = lua_tointeger(p_lua_state, nres);
			break;
		case 's':
			if(!lua_isstring(p_lua_state, nres))
				lua_Error_msg("wrong type.");
			*va_arg(vl, const char**) = lua_tostring(p_lua_state, nres);
			break;
		default:
			break;
		}
		++nres;
	}

	va_end(vl);
}
void LuaCommunicator::Register_CFunction(const char* in_Lua_Variable, lua_CFunction reg_functions)
{
	CCAssert(p_lua_state != nullptr, "Can't Register_CFuntion. p_lua_state is nullptr");
	lua_register(p_lua_state, in_Lua_Variable, reg_functions);
}
lua_State* LuaCommunicator::Lua_GetState()
{
	return p_lua_state;
}
void LuaCommunicator::Lua_FileClose()
{
	lua_replace(p_lua_state, LUA_GLOBALSINDEX);
	lua_close(p_lua_state);
	p_lua_state = nullptr;
}
int LuaCommunicator::getTableSize(int table_index)
{
	int ret_cnt = 0;
	/* table is in the stack at index 't' */
	lua_pushnil(p_lua_state);  /* first key */
	while (lua_next(p_lua_state, table_index) != 0) {
		lua_pop(p_lua_state, 1);
		++ret_cnt;
	}

	return ret_cnt;
}
void LuaCommunicator::lua_Error_msg(const char *fmt, ...)
{
	char cocos_log_message[256] = {0,};

	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	vsprintf(cocos_log_message, fmt, argp);
	BD_CCLog("%s", cocos_log_message);
	va_end(argp);
	lua_close(p_lua_state);
	exit(EXIT_FAILURE);
}