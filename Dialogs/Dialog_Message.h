#ifndef DIALOG_MESSAGE_H
#define DIALOG_MESSAGE_H

#include"GameDialog.h"
#include"GameText.h"
#include"GameButton.h"

//消息对话框,用于显示简单的消息提示
class Dialog_Message:public GameDialog{
public:
	Dialog_Message();
	~Dialog_Message();

	//主要控件
	GameText mGameText;//文本内容,显示给玩家需要看的信息
	GameButtonGroup_ConfirmCancel buttonsConfirmCancel;
	//设置内容
	void setText(const string &text);
	void setConfirmCallback(GameButton::ClickCallback callback);
	static uint maxLineCharAmount;//最大行字符数,超出此行宽会自动换行
};
#endif