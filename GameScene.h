#ifndef GAMESCENE_H
#define GAMESCENE_H

#include"GameSprite.h"
#include"GameCamera.h"

/**游戏场景,可以当作一种特殊的精灵*/
class GameScene:public GameSprite{
public:
	GameScene();
	~GameScene();

	GameCamera gameCamera;//场景一般带有一个摄像机,在某些场合下,可以考虑多个摄像机
};
#endif