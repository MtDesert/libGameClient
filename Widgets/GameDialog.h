#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include"GameText.h"
#include"GameButton.h"

/*游戏对话框，用于在游戏中显示一些提示信息
对话框的特点就是有一个框,中间放各种内容*/
class GameDialog:public GameSprite{
public:
	GameDialog();
	~GameDialog();
};
#endif