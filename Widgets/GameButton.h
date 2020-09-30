#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include"GameString.h"

/*游戏中的按钮,可以响应鼠标或触摸事件*/
class GameButton:public GameSprite{
public:
	GameButton();
	~GameButton();

	//成员变量
	bool isPressed;//是否处于按下状态
	//回调函数(button用于指示哪个按钮被按下)
	typedef function<void()> ClickCallback;
	ClickCallback onClicked;

	//以下函数有可能子类差异化
	virtual bool mouseMove(int x,int y);//鼠标移动出区域的时候恢复为不按下状态
	virtual bool mouseKey(MouseKey key,bool pressed);//让按钮可以响应鼠标点击

	virtual void setIsPressed(bool pressed);//设置按下状态,显示出按下效果
};

//自带string的按钮
class GameButton_String:public GameButton{
protected:
	GameString mGameString;//按钮上的文字
public:
	GameButton_String();
	~GameButton_String();

	void setString(const string &str,bool translate=false);//设置字符串内容
	void resizeAsString();//调整成和字符串一样的大小
	virtual void setIsPressed(bool pressed);
};

//"确定""取消"按钮组
class GameButtonGroup_ConfirmCancel:public GameSprite{
public:
	GameButtonGroup_ConfirmCancel();

	typedef GameButton::ClickCallback ClickCallback;

	GameButton_String buttonConfirm,buttonCancel;//确定按钮,取消按钮
	void showButtonCancel(bool show);//是否显示取消按钮
	void setConfirmCancelFunction(ClickCallback confirmCallback,ClickCallback cancelCallback);//设置"确定""取消"函数
};
#endif