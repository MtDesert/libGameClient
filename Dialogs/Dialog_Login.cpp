#include"Dialog_Login.h"
#include"Game.h"

Dialog_Login::Dialog_Login(){
	const SizeType itemWidth=400,spacing=16;
	//内容
	inputBoxUsername.setLabel_Spacing_MaxWidth("Username",true,spacing,itemWidth);
	inputBoxPassword.setLabel_Spacing_MaxWidth("Password",true,spacing,itemWidth);
	inputBoxPassword.inputBox.passwordChar='*';
	//添加
	addSubObject(&inputBoxUsername);
	addSubObject(&inputBoxPassword);
	addSubObject(&buttonsConfirmCancel);
	//调整布局
	size.x = itemWidth + spacing*2;
	allSubObjects_verticalLayout(spacing);
}

void Dialog_Login::setIsRegister(bool b){
	buttonsConfirmCancel.buttonConfirm.setString(b?"Register":"Login",true);
}