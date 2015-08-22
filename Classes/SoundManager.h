#pragma once
#include <map>
#include <stdio.h>
#include <string.h>
class SoundManager
{
	SoundManager(void);
public:
	~SoundManager(void);
public:
	static SoundManager* Instance()
	{
		static SoundManager* singleton_instance = nullptr;
		if(singleton_instance == nullptr)
			singleton_instance = new SoundManager;
		
		return singleton_instance;
	}

public:
	void add_effect_id(const char* key, unsigned int eff_id) {  effect_sound_id_list[key] = eff_id; }
	unsigned int find_effect_id(const char* key)
	{
		unsigned int ret = 0;

		auto begin = effect_sound_id_list.begin();
		auto end = effect_sound_id_list.end();
		auto i = effect_sound_id_list.begin();

		for(i = begin; i != end; ++i)
		{
			if(!strcmp(i->first, key))
			{
				ret = i->second;
				break;
			}
		}
		return ret;
	}
public:
	void register_SoundFunction();
		
	void setBackgroundVolume(int val);
	float getBackgroundVolume();

	void setEffectVolume(int val);
	float getEffectVolume();

	void preload_BackGroundMusic(const char* path);
	void preload_EffectMusic(const char* path);

	void play_BackgroundMusic(const char* path, bool isLoop);
	void stop_BackgroundMusic();
	void pause_BackgroundMusic();
	void resume_BackgroundMusic();

	void play_EffectMusic(const char* path);
	void stop_EffectMusic(const char* path);
	void pause_EffectMusic(const char* path);
	void resume_EffectMusic(const char* path);
	
	void stop_AllEffectMusic();
	void pause_AllEffectMusic();
	void resume_AllEffectMusic();

	void destroy_EffectMusic(const char* path);

	void destroy_AllSounds();
private:
	std::map<const char*, unsigned int>effect_sound_id_list;
	bool isEffectPaused, isBackGroundPaused;
};