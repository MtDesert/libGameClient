#ifndef GAMEDIALOG_MESSAGE_H
#define GAMEDIALOG_MESSAGE_H

#include"GameDialog.h"

//消息对话框,用于显示简单的消息提示
class GameDialog_Message:public GameDialog{
public:
	GameDialog_Message();
	~GameDialog_Message();

	virtual Point2D<float> sizeF()const;//根据内容动态计算尺寸
	//主要控件
	GameText mGameText;//文本内容,显示给玩家需要看的信息
	GameButton_String mGameButton;//按钮,用来给玩家进行操作
	//设置内容
	void setText(const string &text);
	static uint maxLineCharAmount;//最大行字符数,超出此行宽会自动换行
};
#endif