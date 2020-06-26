#include"Dialog_Login.h"
#include"Game.h"

Dialog_Login::Dialog_Login():whenConfirm(nullptr){
	const SizeType itemWidth=400,spacing=16;
	//内容
	inputBoxUsername.setLabelName_ValueWidth_MaxWidth("Username",true,280,itemWidth);
	inputBoxPassword.setLabelName_ValueWidth_MaxWidth("Password",true,280,itemWidth);
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
	auto &btn=buttonsConfirmCancel.buttonConfirm;
	btn.setString(b?"Register":"Login",true);
	if(b){
		btn.onClicked=[&](){//发送注册数据
			auto client=Game::currentClient();
			client->reqRegister(inputBoxUsername.inputBox.mString,inputBoxPassword.inputBox.mString);//请求注册
		};
	}else{
	}
}