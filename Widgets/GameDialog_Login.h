#ifndef GAMEDIALOG_LOGIN_H
#define GAMEDIALOG_LOGIN_H

#include"GameDialog.h"
#include"GameStringInputBox.h"

//登陆对话框
class GameDialog_Login:public GameDialog{
public:
	GameDialog_Login();
	//控件部分
	GameString strUsername;
	GameString strPassword;
	GameStringInputBox inputBoxUsername;
	GameStringInputBox inputBoxPassword;
	GameButton_String buttonConfirm;
	GameButton_String buttonCancel;

	void setIsRegister(bool b);
};
#endif