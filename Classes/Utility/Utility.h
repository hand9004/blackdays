#pragma once
#include "../UI/UI_Def.h"

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define BD_CCLog cocos2d::CCLog
#define BD_CrtDumpMemoryLeaks _CrtDumpMemoryLeaks
#else
#define BD_CCLog
#endif

#define SAFE_DELETE(x) \
{ \
	if(x != nullptr) \
	{ \
		delete x; \
		x = nullptr; \
	}\
}

#define SAFE_DELETE_ARRAY(x) \
{ \
	if(x != nullptr) \
	{ \
		delete []x; \
		x = nullptr; \
	}\
}

// int형의 자료형을 boolean으로 변경시 생기는 성능 경고 warning을 위한 매크로.
#define INTEGER_TO_BOOLEAN(dest_boolean, src_integer) \
{ \
	dest_boolean = (src_integer != 0) ? true : false; \
}

inline long get_ms_onSystem() {
	cocos2d::cc_timeval temp_val;
	long ret_val = 0, micro_sec = 0;
	cocos2d::CCTime::gettimeofdayCocos2d(&temp_val, nullptr);
	micro_sec = (temp_val.tv_sec) * 1000000 + ((int)temp_val.tv_usec);
	ret_val = (micro_sec / 1000);

	return ret_val;
}

// Reserved.
template<class T>
inline void CCObject_Retain_Vector_List(std::vector<T>&list) 
{ 
	unsigned int list_size = list.size(); 
	for(unsigned int i = 0; i < list_size; ++i) 
	{ 
		T obj_iter = list.at(i); 
		obj_iter.retain(); 
	} 
}

// Reserved.
template<class T>
inline void CCObject_Release_Vector_List(std::vector<T>&list) 
{ 
	unsigned int list_size = list.size(); 
	for(unsigned int i = 0; i < list_size; ++i) 
	{ 
		T obj_iter = list.at(i); 
		obj_iter.release(); 
	} 
}

template<class T>
inline void vector_clear(std::vector<T>& vecObj)
{
	vecObj.clear();

	std::vector<T> tempObj;
	
	tempObj.swap(vecObj);
}

template<class T>
inline void vector_clear_heap(std::vector<T>& vecObj)
{
	std::vector<T> tempObj;

	unsigned int vec_size = vecObj.size();

	for(int i = 0; i < vec_size; ++i)
	{
		T vec_iter = vecObj.at(i);
		SAFE_DELETE(vec_iter);
	}
	vecObj.clear();

	tempObj.swap(vecObj);
}

template<class T1, class T2>
inline void map_clear(std::map<T1, T2>& vecObj)
{
	vecObj.clear();

	std::map<T1, T2> tempObj;

	tempObj.swap(vecObj);
}

template<class T1, class T2>
inline void map_clear_heap(std::map<T1, T2>& vecObj)
{
	std::map<T1, T2> tempObj;

	auto begin = vecObj.begin();
	auto end = vecObj.end();

	for(; begin != end; ++begin)
	{
		SAFE_DELETE(begin->second);
	}
	vecObj.clear();

	tempObj.swap(vecObj);
}

#define MASK_VALUE_4BIT 0xF
#define PI 3.1415926535897932384626433832795
