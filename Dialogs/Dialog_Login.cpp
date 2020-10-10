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
	//设置事件
	auto client=Game::currentGame()->gameClient;
	if(client){
		client->whenRegisterOK=[&](SocketDataBlock &sdb){//注册成功
			Game::dialogOK("Register OK!");
		};
		client->whenLoginOK=[&](SocketDataBlock &sdb){//登陆成功
			Game::dialogOK("Login OK!");
		};
	}
}

void Dialog_Login::setIsRegister(bool b){
	auto &btn=buttonsConfirmCancel.buttonConfirm;
	btn.setString(b?"Register":"Login",true);
	auto game=Game::currentGame();
	auto client=game->gameClient;
	if(client){
		if(b){
			btn.onClicked=[&,game,client](){//发送注册数据
				client->reqRegister(game->gameName(),inputBoxUsername.inputBox.mString,inputBoxPassword.inputBox.mString);
			};
		}else{
			btn.onClicked=[&,game,client](){
				client->reqLogin(game->gameName(),inputBoxUsername.inputBox.mString,inputBoxPassword.inputBox.mString);
			};
		}
	}
}