#include"GameScene.h"
#include"Game.h"
#include"GameDialog_Login.h"

//对话框
static GameDialog_Login *gameDialog_login=nullptr;

#define GET_USERNAME_PASSWORD \
auto name=gameDialog_login->inputBoxUsername.mString;\
auto pass=gameDialog_login->inputBoxPassword.mString;
//注册
static void doRegister(GameButton*){
	GET_USERNAME_PASSWORD
}
//登陆
static void doLogin(GameButton*){
	GET_USERNAME_PASSWORD
	Game::currentClient()->reqLogin(name,pass);
}
//取消登陆
static void cancelLogin(GameButton*){
	auto scene=dynamic_cast<GameScene*>(gameDialog_login->parentObject);
	if(scene){
		scene->hideLoginDialog();
	}
}

GameScene::GameScene(){
	gameCamera.size=Game::currentGame()->resolution;
}
GameScene::~GameScene(){
	delete gameDialog_login;
}

void GameScene::showLoginDialog(bool isRegister){
	if(!gameDialog_login){
		gameDialog_login=new GameDialog_Login();
		gameDialog_login->buttonCancel.onClicked=cancelLogin;
	}
	gameDialog_login->setIsRegister(isRegister);
	gameDialog_login->buttonConfirm.onClicked=isRegister ? doRegister : doLogin;
	addSubObject(gameDialog_login);
}
void GameScene::hideLoginDialog(){
	removeSubObject(gameDialog_login);
}