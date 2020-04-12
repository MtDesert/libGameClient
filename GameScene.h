#ifndef GAMESCENE_H
#define GAMESCENE_H

#include"GameSprite.h"
#include"GameCamera.h"

/**游戏场景,可以当作一种特殊的精灵*/
class GameScene:public GameSprite{
	enum FadeStatus{//淡入淡出状态
		NotFading,FadingIn,FadingOut
	};
	FadeStatus fadeStatus;
public:
	GameScene();
	~GameScene();

	void consumeTimeSlice();

	GameCamera gameCamera;//场景一般带有一个摄像机,在某些场合下,可以考虑多个摄像机

	//淡入淡出
	void fadeIn();//淡入场景
	void fadeOut();//淡出场景
	void stopFading();//停止淡入淡出
};
#endif