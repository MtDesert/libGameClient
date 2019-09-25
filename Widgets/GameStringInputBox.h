#ifndef GAMESTRINGINPUTBOX_H
#define GAMESTRINGINPUTBOX_H

#include"GameButton.h"
//字符串输入框,用于输入文字
class GameStringInputBox:public GameButton_String{
public:
	GameStringInputBox();

	string mString;//字符串内容,通过判断此内容来获得字符串的值
	char passwordChar;//密码字符,'\0'为正常显示
	void setString(const string &str);
	void startInput();//开始输入,这里可以调用系统的输入方式
};
#endif