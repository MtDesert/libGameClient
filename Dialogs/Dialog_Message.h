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
	GameButtonGroup_ConfirmCancel buttonsConfirmCancel;//确认取消按钮,用于给玩家选择
	function<void()> confirmCallback;//确认回调函数,点击确认后调用此函数
	//设置内容
	void setText(const string &text);//设置文本
	void setConfirmCallback(GameButton::ClickCallback callback);//设置点击确定后的回调函数
	static uint maxLineCharAmount;//最大行字符数,超出此行宽会自动换行
};
#endif