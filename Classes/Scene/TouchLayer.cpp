#include "TouchLayer.h"
#include "../StageManager.h"
#include "../UI/UIController.h"
#include "../Objects/ObjectController.h"
#include "../Objects/Map.h"
#include "SceneManager.h"

USING_NS_CC;

const float scroll_speed_limit = 15.0f;
const float scroll_decrease = 0.5f;

// on "init" you need to initialize your instance
bool TouchLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	schedule(schedule_selector(TouchLayer::check_touch_event));

    return true;
}
void TouchLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool TouchLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	UIController::Instance()->setTouchBegan(pTouch, pEvent);
	if(!UIController::Instance()->getIsTouchGrab())
	{
		Map::Instance()->setTouchBegan(pTouch, pEvent);
		ObjectController::Instance()->setTouchBegan(pTouch, pEvent);
	}
	scroll_delta.setPoint(0.0f, 0.0f);

	return true;
}
void TouchLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	UIController::Instance()->setTouchMoved(pTouch, pEvent);
	if(!UIController::Instance()->getIsTouchGrab())
	{
		Map::Instance()->setTouchMoved(pTouch, pEvent);
		ObjectController::Instance()->setTouchMoved(pTouch, pEvent);
	}

	cocos2d::CCPoint current_pt = pTouch->getLocation();
	cocos2d::CCPoint prev_pt = pTouch->getPreviousLocation();
	scroll_delta.setPoint(current_pt.x - prev_pt.x, current_pt.y - prev_pt.y);
}
void TouchLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	UIController::Instance()->setTouchEnded(pTouch, pEvent);
	if(!UIController::Instance()->getIsTouchGrab())
	{
		Map::Instance()->setTouchEnded(pTouch, pEvent);
		ObjectController::Instance()->setTouchEnded(pTouch, pEvent);
	}
	else
		UIController::Instance()->setIsTouchGrab(false);
}
void TouchLayer::keyBackClicked()
{
	SceneManager::Instance()->destroyScene();

	cocos2d::CCFileUtils::sharedFileUtils()->purgeCachedEntries();
	cocos2d::CCFileUtils::sharedFileUtils()->purgeFileUtils();
	cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	cocos2d::CCDirector::sharedDirector()->popScene();
	cocos2d::CCDirector::sharedDirector()->end();

	cocos2d::CCEGLView::sharedOpenGLView()->end();
	BD_CCLog("keyBackClicked.");
}
void TouchLayer::keyMenuClicked()
{
	BD_CCLog("keyMenuClicked.");
}
void TouchLayer::check_touch_event(float dt)
{
	scene_type current_scene_type = SceneManager::Instance()->get_SceneType();

	switch(current_scene_type)
	{
	case SELECT:
		
		break;
	case GAME:
		if(StageManager::Instance()->getStageUpdateLock())
			this->setTouchEnabled(false);
		else
		{
			this->setTouchMode(kCCTouchesOneByOne);
			this->setTouchEnabled(true);

			if(!UIController::Instance()->getIsTouchGrab())
			{
				if(scroll_delta.x > scroll_speed_limit)
					scroll_delta.x = scroll_speed_limit;
				else if(scroll_delta.x < -scroll_speed_limit)
					scroll_delta.x = -scroll_speed_limit;

				if(scroll_delta.x > scroll_decrease)
					scroll_delta.x -= scroll_decrease;
				else if(scroll_delta.x < -scroll_decrease)
					scroll_delta.x += scroll_decrease;
				else
					scroll_delta.x = 0.0f;

				// 검사전 맵을 업데이트 하고, 업데이트 후, 검사함으로써, 좌표를 다시 제대로 잡는다.
				// 그렇게 하여, 움직일 수 있는 경우면, 오브젝트 또한 갱신을 시도한다.
				if (!SceneManager::Instance()->getIsCameraFixed())
				{
					Map::Instance()->setUpdateScrolling(scroll_delta.x);
					if (Map::Instance()->getIsCanScrolling())
						ObjectController::Instance()->setUpdateScrolling(scroll_delta.x);
				}
			}
		}
		break;
	default:
		this->setTouchMode(kCCTouchesOneByOne);
		this->setTouchEnabled(true);
		break;
	}
}
