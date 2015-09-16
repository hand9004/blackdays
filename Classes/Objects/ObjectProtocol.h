#pragma once
#include <map>
#include <vector>
#include <cocos2d.h>

typedef struct GRAPHIC_TO_OBJECT_ANIMATE
{
	unsigned int current_index;
	bool isAnimationFinish;
}grap_to_obj_animate;

typedef struct GRAPHIC_TO_OBJECT_NORMAL
{
}grap_to_obj_normal;

typedef struct GRAPHIC_TO_OBJECT_PACKET
{
	void* data;
}grap_to_obj_packet;

typedef enum OBJECT_EVENT
{
	NO_EVENT,
	MOVE,
	ATTACK,
	SKILL,
	PATROL,
	SEARCHING_RECOGNIZE_AREA,
	SLEEP,
	STUN,
	DEAD,
}obj_event;

// ��ƼŬ ����
typedef enum PARTICLE_TYPE
{
	FIRE,
	FIREWORKS,
	SUN,
	GALAXY,
	FLOWER,
	METEOR,
	SPIRAL,
	EXPLOSION,
	SMOKE,
	SNOW,
	RAIN,
}particle_type;

typedef struct PARTICLE_DATA
{
	const char *particle_name, *texture_path;
	particle_type parti_type;
	cocos2d::CCPoint particle_system_pos, gravity_mass;
	float life_val, emission_rate;
}particle_data;

typedef struct IN_PARTICLE_DATA
{
	particle_data* particle_dat;
	cocos2d::CCParticleSystem* particle_sys;
}in_particle_data;

// ����Ʈ ����
typedef enum EFFECT_DRAW_TYPE
{
	DRAW_TO_ME,
	DRAW_TO_ME_RANDOM,
	DRAW_TO_TARGET,
}eff_draw_type;

typedef struct EFFECT_SET_INFO
{
	const char* key;
	eff_draw_type draw_type;
	unsigned int applying_index;
	cocos2d::CCPoint relative_distance;
}effect_set_info;

typedef struct EFFECT_INFO
{
	const char* effect_list_name, *grap_object_key;
	std::vector<effect_set_info*>effect_set_list;
}effect_info;

// �ִϸ��̼� ����
typedef struct IMAGEFRAME_INFO
{
	const char* base_image, *key;
	cocos2d::CCRect in_image_area;
}imgframe_info;

typedef struct IN_ANIMATEFRAME_INFO
{
	unsigned int frame;
	const char* in_list_key;
}in_aniframe_info;

typedef struct ANIMATEFRAME_INFO
{
	const char* ref_image_name, *ref_list_name, *ani_name;
	std::vector<in_aniframe_info*>in_aniframe_list;
}aniframe_info;

typedef struct ANIMATEFRAME_SET_INFO
{
	float delay;
	bool isOneTimePlay;
	const char* ani_name;
	unsigned int rep_start, rep_end;
}aniframe_set_info;

// ��ų ����
typedef struct SKILL_INFO
{
	// ��Ʒκ��� �Է� �޴� �⺻ ����
	const char* skill_image_name, *ref_ani_name, *skill_name, *attack_type, *skill_type;
	std::vector<particle_data*>skill_particle_list;
	std::vector<unsigned int>chain_hit_in_ani_frame_list;
	float power_percentage, skill_cooldown, global_vibration;
	unsigned int charge_dist;

	// ���� �����󿡼� ������ ����
	bool isSkillUsed;
	long cooldn_start_time, cooldn_end_time, cooldn_elapsed_time;
}skill_info;

// ���� ��ü ����
typedef struct OBJECT_INFO
{
	int health_point;
	unsigned int max_health_point;
	unsigned int attack_point;
	unsigned int defense_point;
	unsigned int recognize_area;
	unsigned int firing_area_X, firing_area_Y;
	float move_speed, patrol_delay_time;
	char object_name[256];
	bool isEnemy, isHero, isControllable, isAreaAttack;
	cocos2d::CCPoint pos;
	std::map<const char*, const char*>main_image;
	std::vector<imgframe_info*>img_frame_list;
	std::vector<aniframe_info*>ani_image_list;
	std::vector<aniframe_set_info*>ani_frame_set_list;
	std::vector<effect_info*>effect_list;
	std::vector<skill_info*>skill_list;
}obj_info;

// ��ũ��Ʈ���� �о�� �����͸� GameGraphicComponent ��ü�� ��翡 ������Ű�� ���� ��ü.
typedef struct MAIN_GRAPHIC_ANI_INFO
{
	effect_info* effect_data;
	aniframe_set_info* ani_frame_set;
	aniframe_info* ani_frame_data;
}main_ani_info;

typedef struct MAIN_GRAPHIC_INFO
{
	imgframe_info* m_imgframe_info;
	main_ani_info* m_ani_info;
}main_grap_info;