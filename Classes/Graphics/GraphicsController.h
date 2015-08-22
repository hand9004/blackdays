#pragma once
#include <algorithm>
#include <cocos2d.h>
#include "../UI/UI_Def.h"

class GraphicsController
{
	GraphicsController(void);
public:
	~GraphicsController(void);
public:
	static GraphicsController* Instance()
	{
		static GraphicsController* singleton_instance = nullptr;
		if(singleton_instance == nullptr)
			singleton_instance = new GraphicsController;
		
		return singleton_instance;
	}
public:
	void getSprite(cocos2d::CCSprite* (&src_spr), const char* file_name, cocos2d::CCPoint* pt);
	void getSprite(cocos2d::CCSprite* (&src_spr), cocos2d::CCSpriteFrame* frame);
	void getSprite(cocos2d::CCSprite* (&src_spr), cocos2d::CCTexture2D* texture);
};