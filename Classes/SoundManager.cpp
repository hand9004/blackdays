#include "SoundManager.h"
#include "LuaCommunicator.h"
#include "Utility\Utility.h"

#include <SimpleAudioEngine.h>
int l_load_background_music(lua_State* L)
{
	const char* bg_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->preload_BackGroundMusic(bg_music_path);
	return 0;
}
int l_load_effect_music(lua_State* L)
{
	const char* eff_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->preload_EffectMusic(eff_music_path);
	return 0;
}

int l_set_background_music_volume(lua_State* L)
{
	int background_volume = lua_tointeger(L, 1);

	SoundManager::Instance()->setBackgroundVolume(background_volume);

	return 0;
}

int l_set_effect_music_volume(lua_State* L)
{
	int effect_volume = lua_tointeger(L, 1);

	SoundManager::Instance()->setEffectVolume(effect_volume);

	return 0;
}

int l_play_background_music(lua_State* L)
{
	bool isLoop = false;
	const char* bg_music_path = lua_tostring(L, 1);
	INTEGER_TO_BOOLEAN(isLoop, lua_toboolean(L, 2));

	SoundManager::Instance()->play_BackgroundMusic(bg_music_path, isLoop);
	
	return 0;
}
int l_stop_background_music(lua_State* L)
{
	SoundManager::Instance()->stop_BackgroundMusic();
	return 0;
}
int l_pause_background_music(lua_State* L)
{
	SoundManager::Instance()->pause_BackgroundMusic();
	return 0;
}
int l_resume_background_music(lua_State* L)
{
	SoundManager::Instance()->resume_BackgroundMusic();
	return 0;
}

int l_play_effect_music(lua_State* L)
{
	const char* eff_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->add_effect_id(eff_music_path, CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(eff_music_path));
	return 0;
}
int l_stop_effect_music(lua_State* L)
{
	const char* eff_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->stop_EffectMusic(eff_music_path);

	return 0;
}
int l_pause_effect_music(lua_State* L)
{
	const char* eff_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->pause_EffectMusic(eff_music_path);

	return 0;
}
int l_resume_effect_music(lua_State* L)
{
	const char* eff_music_path = lua_tostring(L, 1);

	SoundManager::Instance()->resume_EffectMusic(eff_music_path);

	return 0;
}

int l_stop_all_effect_music(lua_State* L)
{
	SoundManager::Instance()->stop_AllEffectMusic();

	return 0;
}

int l_pause_all_effect_music(lua_State* L)
{
	SoundManager::Instance()->pause_AllEffectMusic();

	return 0;
}

int l_resume_all_effect_music(lua_State* L)
{
	SoundManager::Instance()->resume_AllEffectMusic();

	return 0;
}

SoundManager::SoundManager(void) : isEffectPaused(false), isBackGroundPaused(false)
{
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::register_SoundFunction()
{
	LuaCommunicator::Instance()->Register_CFunction("load_background_music", l_load_background_music);
	LuaCommunicator::Instance()->Register_CFunction("load_effect_music", l_load_effect_music);

	LuaCommunicator::Instance()->Register_CFunction("set_background_music_volume", l_set_background_music_volume);
	LuaCommunicator::Instance()->Register_CFunction("set_effect_music_volume", l_set_effect_music_volume);

	LuaCommunicator::Instance()->Register_CFunction("play_background_music", l_play_background_music);
	LuaCommunicator::Instance()->Register_CFunction("stop_background_music", l_stop_background_music);
	LuaCommunicator::Instance()->Register_CFunction("pause_background_music", l_pause_background_music);
	LuaCommunicator::Instance()->Register_CFunction("resume_background_music", l_resume_background_music);

	LuaCommunicator::Instance()->Register_CFunction("play_effect_music", l_play_effect_music);
	LuaCommunicator::Instance()->Register_CFunction("stop_effect_music", l_stop_effect_music);
	LuaCommunicator::Instance()->Register_CFunction("pause_effect_music", l_pause_effect_music);
	LuaCommunicator::Instance()->Register_CFunction("resume_effect_music", l_resume_effect_music);

	LuaCommunicator::Instance()->Register_CFunction("stop_all_effect_music", l_stop_all_effect_music);
	LuaCommunicator::Instance()->Register_CFunction("pause_all_effect_music", l_pause_all_effect_music);
	LuaCommunicator::Instance()->Register_CFunction("resume_all_effect_music", l_resume_all_effect_music);
}
void SoundManager::setBackgroundVolume(int val)
{
	float scale_val = (float)(val / 100.f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(scale_val);
}
float SoundManager::getBackgroundVolume()
{
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
}
void SoundManager::setEffectVolume(int val)
{
	float scale_val = (float)(val / 100.f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(scale_val);
}
float SoundManager::getEffectVolume()
{
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume();
}
void SoundManager::preload_BackGroundMusic(const char* path)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(path);
}
void SoundManager::preload_EffectMusic(const char* path)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(path);
}

void SoundManager::play_BackgroundMusic(const char* path, bool isLoop)
{
	if(!isBackGroundPaused)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path, isLoop);
}
void SoundManager::stop_BackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void SoundManager::pause_BackgroundMusic() 
{
	isBackGroundPaused = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
void SoundManager::resume_BackgroundMusic()
{
	isBackGroundPaused = false;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void SoundManager::play_EffectMusic(const char* path)
{
	if(!isEffectPaused)
		add_effect_id(path, CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(path));
}
void SoundManager::stop_EffectMusic(const char* path)
{
	unsigned int finded_id = find_effect_id(path);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(finded_id);
}
void SoundManager::pause_EffectMusic(const char* path)
{
	unsigned int finded_id = find_effect_id(path);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(finded_id);
}
void SoundManager::resume_EffectMusic(const char* path)
{
	unsigned int finded_id = find_effect_id(path);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(finded_id);
}
void SoundManager::stop_AllEffectMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
void SoundManager::pause_AllEffectMusic()
{
	isEffectPaused = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}
void SoundManager::resume_AllEffectMusic()
{
	isEffectPaused = false;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
void SoundManager::destroy_EffectMusic(const char* path)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(path);
}
void SoundManager::destroy_AllSounds()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	auto begin = effect_sound_id_list.begin();
	auto end = effect_sound_id_list.end();
	auto i = effect_sound_id_list.begin();

	for(auto i = begin; i != end; ++i)
		SoundManager::Instance()->stop_EffectMusic(i->first);

	map_clear(effect_sound_id_list);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}