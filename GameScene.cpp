#include"GameScene.h"
#include"Game.h"
#include"GameDialog_Message.h"
#include"GameDialog_Login.h"

//对话框
static GameDialog_Message *gameDialog_message=nullptr;
static GameDialog_Login *gameDialog_login=nullptr;

GameScene::GameScene(){
	gameCamera.size=Game::currentGame()->resolution;
}

//static void hideGameDialog(){Game::currentGame()->hideDialogMessage();}
void GameScene::showDialogMessage(const string &content){
	if(!gameDialog_message){
		gameDialog_message=new GameDialog_Message();
		gameDialog_message->setText(content);
	}
	addSubObject(gameDialog_message);
}
void GameScene::hideDialogMessage(){
	removeSubObject(gameDialog_message);
	delete gameDialog_message;
	gameDialog_message=nullptr;
}
void GameScene::showLoginDialog(){
	if(!gameDialog_login){
		gameDialog_login=new GameDialog_Login();
	}
	addSubObject(gameDialog_login);
}
void GameScene::hideLoginDialog(){
}