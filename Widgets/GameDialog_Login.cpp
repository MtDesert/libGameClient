#include"GameDialog_Login.h"
#include"Game.h"

#define ENG(name) #name

GameDialog_Login::GameDialog_Login(){
	size.setXY(400,160);
	//内容
	strUsername.setString("Username");
	strPassword.setString("Password");
	buttonCancel.setString("Cancel");
	inputBoxPassword.passwordChar='*';
	//位置锚点
	strUsername.position.x=strPassword.position.x=-100;
	strUsername.position.y=50;
	strUsername.anchorPoint.x=strPassword.anchorPoint.x=1;

	inputBoxUsername.position.x = inputBoxPassword.position.x = strUsername.position.x+10;
	inputBoxUsername.position.y = strUsername.position.y;
	inputBoxUsername.anchorPoint.x=inputBoxPassword.anchorPoint.x=0;

	buttonConfirm.position.x=-100;
	buttonCancel.position.x=100;
	buttonConfirm.position.y = buttonCancel.position.y = -50;
	//添加
	addSubObject(&strUsername);
	addSubObject(&strPassword);
	addSubObject(&inputBoxUsername);
	addSubObject(&inputBoxPassword);
	addSubObject(&buttonConfirm);
	addSubObject(&buttonCancel);
}

void GameDialog_Login::setIsRegister(bool b){
	buttonConfirm.setString(b?"Register":"Login");
}