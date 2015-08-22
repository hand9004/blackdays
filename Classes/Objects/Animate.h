#pragma once
#include <cocos2d.h>
#include <vector>
#include <time.h>
#include "GameGraphicComponent.h"
class Animate : public GameGraphicComponent
{
public:
	Animate(void);
	~Animate(void);
public:
	virtual void Init(main_grap_info& game_graphic_info);
	virtual void Update();
	virtual void Destroy();
	virtual void SetPacket(grap_to_obj_packet& src_packet);
private:
	void animation_Play();
	void animation_Stop();
	void check_AnimationEnd();
	void set_Repeat_Area(unsigned int rep_start, unsigned int rep_end);
	void set_Delay(float delay);
	void setCollisionRect();
private:
	bool isOnePlay, isAnimateEnd, canSendMessage;
	const char* ani_name;
	unsigned int rep_start_index, rep_end_index;
	std::vector<cocos2d::CCSpriteFrame*>anim_frame_list;
	int start_time, end_time;
	float frame_interval_time;
	unsigned long ani_index;
};