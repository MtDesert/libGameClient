#include"GameScene.h"
#include"Game.h"

GameScene::GameScene():fadeStatus(NotFading){
	gameCamera.size=Game::currentGame()->resolution;
}
GameScene::~GameScene(){
	Game::currentGame()->timeSliceList.removeTimeSlice(this);
}

void GameScene::consumeTimeSlice(){
	switch(fadeStatus){//淡入淡出
		case FadingIn://淡入
			color.alpha+=5;
			if(color.alpha>=0xFF)stopFading();
		break;
		case FadingOut:
			--color.alpha;
			if(color.alpha<=0)stopFading();
		break;
		default:;
	}
}

void GameScene::fadeIn(){
	fadeStatus=FadingIn;
	Game::currentGame()->timeSliceList.pushTimeSlice(this,20,40);
}
void GameScene::fadeOut(){
	fadeStatus=FadingOut;
	Game::currentGame()->timeSliceList.pushTimeSlice(this,20,40);
}
void GameScene::stopFading(){
	fadeStatus=NotFading;
	auto game=Game::currentGame();
	game->timeSliceList.removeTimeSlice(this);
	game->scenarioScript->scriptContinue();//让脚本继续执行
}