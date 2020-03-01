#include"GameScene.h"
#include"Game.h"

GameScene::GameScene(){
	gameCamera.size=Game::currentGame()->resolution;
}
GameScene::~GameScene(){}