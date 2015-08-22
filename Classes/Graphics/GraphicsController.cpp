#include "GraphicsController.h"
#include "../Utility/Utility.h"
#include "../Scene/SceneManager.h"
#include "../LuaCommunicator.h"
#include "../UI/UIController.h"

USING_NS_CC;

GraphicsController::GraphicsController(void)
{
}

GraphicsController::~GraphicsController(void)
{
}
void GraphicsController::getSprite(cocos2d::CCSprite* (&src_spr), const char* file_name, cocos2d::CCPoint* pt)
{
	CCAssert(file_name != nullptr, "Can't Get Sprite.");
	src_spr = cocos2d::CCSprite::create(file_name);
	src_spr->setPosition(*pt);
}
void GraphicsController::getSprite(cocos2d::CCSprite* (&src_spr), cocos2d::CCSpriteFrame* frame)
{
	src_spr = cocos2d::CCSprite::createWithSpriteFrame(frame);
}
void GraphicsController::getSprite(cocos2d::CCSprite* (&src_spr), cocos2d::CCTexture2D* texture)
{
	src_spr = cocos2d::CCSprite::createWithTexture(texture);
}