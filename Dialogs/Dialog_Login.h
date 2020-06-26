#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include"GameDialog.h"
#include"GameInputBox.h"

//登陆对话框
class Dialog_Login:public GameDialog{
public:
	Dialog_Login();
	//控件部分
	GameAttr_InputBoxString inputBoxUsername;
	GameAttr_InputBoxString inputBoxPassword;
	GameButtonGroup_ConfirmCancel buttonsConfirmCancel;
	//设置模式(false登陆,true注册)
	void setIsRegister(bool b);
	GameButton::ClickCallback whenConfirm;//确认后的回调
};
#endif