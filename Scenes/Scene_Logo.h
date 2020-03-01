#ifndef SCENE_LOGO_H
#define SCENE_LOGO_H

#include"GameScene.h"
#include"GameText.h"

//显示游戏标志,本类只简单显示文本
class Scene_Logo:public GameScene{
	int status,countDown;
public:
	Scene_Logo();
	~Scene_Logo();

	GameString logoText;//标志文本
	function<void()> whenLogoOver;//标志结束后的动作

	virtual void reset();
	virtual void consumeTimeSlice();
};
#endif