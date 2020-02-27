#ifndef GAMEDIALOG_LOGIN_H
#define GAMEDIALOG_LOGIN_H

#include"GameDialog.h"
#include"GameInputBox.h"

//登陆对话框
class GameDialog_Login:public GameDialog{
public:
	GameDialog_Login();
	//控件部分
	GameAttr_InputBoxString inputBoxUsername;
	GameAttr_InputBoxString inputBoxPassword;
	GameButtonGroup_ConfirmCancel buttonsConfirmCancel;

	void setIsRegister(bool b);
};
#endif