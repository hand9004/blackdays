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
	cocos2d::CCScene* getCurrentScene();
	void clearScene();

	void setIsCameraFixed(bool isCameraFixed) { this->isCameraFixed = isCameraFixed; }
	void setIsCameraReset(bool isCameraReset) { this->isCameraReset = isCameraReset; }
	void set_in_UI_CameraFixed(bool in_UI_IsCameraFixed) { this->in_UI_IsCameraFixed = in_UI_IsCameraFixed; }

	bool getIsCameraFixed() { return isCameraFixed; }
	bool getIsCameraReset() { return isCameraReset; }
	bool get_in_UI_CameraFixed() { return in_UI_IsCameraFixed; }


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
private:
	scene_type current_scene_type;
	cocos2d::CCScene* current_Scene;
	const char* replace_scene_token;
	bool isCameraFixed, isCameraReset, in_UI_IsCameraFixed;
};

