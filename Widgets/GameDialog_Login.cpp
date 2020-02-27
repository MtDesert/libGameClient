#include"GameDialog_Login.h"
#include"Game.h"

#define ENG(name) #name

GameDialog_Login::GameDialog_Login(){
	size.setXY(400,160);
	//内容
	inputBoxUsername.setLabel_Spacing_MaxWidth("Username",true,16,size.x);
	inputBoxPassword.setLabel_Spacing_MaxWidth("Password",true,16,size.x);
	inputBoxPassword.inputBox.passwordChar='*';
	//添加
	addSubObject(&inputBoxUsername);
	addSubObject(&inputBoxPassword);
	addSubObject(&buttonsConfirmCancel);
}

void GameDialog_Login::setIsRegister(bool b){
	buttonsConfirmCancel.buttonConfirm.setString(b?"Register":"Login",true);
}