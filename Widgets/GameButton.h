#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include"GameSprite.h"
#include"GameString.h"

/*游戏中的按钮,可以响应鼠标或触摸事件*/
class GameButton:public GameSprite{
public:
	GameButton();
	~GameButton();

	//成员变量
	bool isPressed;//是否处于按下状态
	//回调函数(button用于指示哪个按钮被按下)
	void (*onClicked)(GameButton *button);

	//以下函数有可能子类差异化
	virtual bool mouseMove(int x,int y);//鼠标移动出区域的时候恢复为不按下状态
	virtual bool mouseKey(MouseKey key,bool pressed);//让按钮可以响应鼠标点击

	bool isMouseOnButton()const;//判断鼠标是否在按钮上
	virtual void setIsPressed(bool pressed);
};

//自带string的按钮
class GameButton_String:public GameButton{
protected:
	GameString mGameString;//按钮上的文字
public:
	GameButton_String();
	~GameButton_String();

	void setString(const string &str,bool translate=false);
	virtual void setIsPressed(bool pressed);
};

#endif