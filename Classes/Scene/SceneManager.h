#pragma once
#include <cocos2d.h>

typedef enum SCENE_TYPE
{
	MAIN,
	SELECT,
	GAME,
}scene_type;

class SceneManager
{
	SceneManager(void);
public:
	~SceneManager(void);
public:
	static SceneManager* Instance()
	{
		static SceneManager* singleton_instance = nullptr;
		if(singleton_instance == nullptr)
			singleton_instance = new SceneManager;
		
		return singleton_instance;
	}

	void register_SceneFunction();
	void createScene(scene_type type);
	void updateScene();
	void destroyScene();
	void clearScene();

	cocos2d::CCScene* getCurrentScene();

	void setIsCameraFixed(bool isCameraFixed) { this->isCameraFixed = isCameraFixed; }
	void setIsGamePaused(bool isGamePaused) { this->isGamePaused = isGamePaused; }
	void setSceneVibration(bool isSceneVibration, long vibrate_duration_ms)
	{
		this->isSceneVibration = isSceneVibration;
		this->vibrate_duration_ms = vibrate_duration_ms;
	}

	bool getIsCameraFixed() { return isCameraFixed; }
	bool getIsGamePaused() { return isGamePaused; }
	bool getIsSceneVibration() { return isSceneVibration; }

	scene_type get_SceneType() { return current_scene_type; }

	bool isSceneChanging()
	{
		if(strcmp(replace_scene_token, ""))
			return true;
		else
			return false;
	}

	void set_SceneReplaceToken(const char* tok) { replace_scene_token = tok; }
	const char* get_SceneReplaceToken() { return replace_scene_token; }
public:
	void control_scheduler(const char* scene_type, bool isPaused);
private:
	void update_scene_vibration();
private:
	scene_type current_scene_type;
	cocos2d::CCScene* current_Scene;
	const char* replace_scene_token;
	bool isCameraFixed, isGamePaused, isSceneVibration;
	long vibrate_start_time, vibrate_end_time, vibrate_duration_ms;
};

