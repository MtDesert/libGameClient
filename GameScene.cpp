#include"GameScene.h"
#include"Game.h"
#include"GameDialog_Login.h"

//对话框
static GameDialog_Login *gameDialog_login=nullptr;

//注册
static void doRegister(){}
//登陆
static void doLogin(){}

GameScene::GameScene(){
	gameCamera.size=Game::currentGame()->resolution;
}
GameScene::~GameScene(){
	delete gameDialog_login;
}

void GameScene::showLoginDialog(bool isRegister){
	if(!gameDialog_login){
		gameDialog_login=new GameDialog_Login();
		gameDialog_login->buttonsConfirmCancel.buttonCancel.onClicked=[&](){
			auto scene=dynamic_cast<GameScene*>(parentObject);
			if(scene){
				scene->hideLoginDialog();
			}
		};
	}
	gameDialog_login->setIsRegister(isRegister);
	gameDialog_login->buttonsConfirmCancel.buttonConfirm.onClicked=isRegister ? doRegister : doLogin;
	addSubObject(gameDialog_login);
}
void GameScene::hideLoginDialog(){
	removeSubObject(gameDialog_login);
}