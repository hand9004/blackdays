#pragma once
#include "ObjectProtocol.h"

typedef struct EFFECT_DRAW_DATA
{
	effect_set_info* eff_set_info;
	cocos2d::CCSpriteFrame* eff_sprFrame;
}eff_draw_data;

class Effect
{
public:
	Effect(void);
	~Effect(void);
public:
	void Init(effect_info* eff_info);
	void Update();
	void Destroy();
public:
	bool isEffectEnd();
	void syncWithEffectTime(unsigned int limit_time_ms);
	void syncWithEffectFrameIndex(unsigned int frame_index);
	void setObjectPos(cocos2d::CCPoint obj_pos) { object_pos = obj_pos; }
	void setTargetPos(cocos2d::CCPoint tar_pos) { target_pos = tar_pos; }
	void setGraphicFlipX(bool isFlip) { isFlipX = isFlip; }
	void setGraphicFlipY(bool isFlip) { isFlipY = isFlip; }
	void setEffectVisible(bool isVisible) { isEffectOn = isVisible; }
private:
	effect_info* eff_info;
	std::vector<eff_draw_data*>effect_frame;
	cocos2d::CCSprite* effect_spr;
	cocos2d::CCPoint object_pos, target_pos;
	unsigned int selected_index;
	bool isEffectOn, isFlipX, isFlipY;
	unsigned long start_time, end_time;
};
