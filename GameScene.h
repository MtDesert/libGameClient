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
	//提示信息
	void showDialogMessage(const string &content);
	void hideDialogMessage();
	//登陆
	void showLoginDialog(bool isRegister=false);//isRegister为true是注册窗口,否则是登陆窗口
	void hideLoginDialog();
};
#endif