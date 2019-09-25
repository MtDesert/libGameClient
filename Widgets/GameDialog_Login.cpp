#include"GameDialog_Login.h"
#include"Game.h"

#define ENG(name) Game::currentGame()->translate(#name)

GameDialog_Login::GameDialog_Login(){
	size.x=400;size.y=160;
	//内容
	strUsername.setString(ENG(Username));
	strPassword.setString(ENG(Password));
	buttonConfirm.setString(ENG(Login));
	buttonCancel.setString(ENG(Cancel));
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