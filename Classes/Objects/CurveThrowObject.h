#pragma once
#include "ObjectProtocol.h"
#include "Effect.h"
class CurveThrowObject
{
public:
	CurveThrowObject(void);
	~CurveThrowObject(void);
public:
	void Init(cocos2d::CCPoint start_pt, cocos2d::CCPoint end_pt);
	void Update();
	void Destroy();
public:
	void change_PosList_OnScrolling(int delta_X);
	bool isInBombArea(cocos2d::CCPoint src_pos);
	void setSendedDamage(bool isSendDamage) { isSendedDamage = isSendDamage; }
	bool getSendedDamage() { return isSendedDamage; }

	bool isArrived() { unsigned int curve_size = throwing_line.size(); return (current_index >= curve_size) ? true : false; }
	bool isDestroy() { unsigned int curve_size = throwing_line.size(); return ((current_index >= curve_size) && isBombEffectEnd) ? true : false; }
private:
	void create_bezier_to_pos(cocos2d::CCPoint start_pt, cocos2d::CCPoint end_pt);
private:
	unsigned int current_index;
	bool isBombEffectEnd, isSoundPlayed, isSendedDamage;
	cocos2d::CCSprite* curve_throw_spr;
	std::vector<cocos2d::CCPoint>throwing_line;
	std::vector<std::string>effect_frame_name_list;
	Effect bomb_explosion_effect;
	float radius_X, radius_Y;
};

