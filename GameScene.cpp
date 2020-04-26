#include"GameScene.h"
#include"Game.h"

static Game *game=nullptr;

GameScene::GameScene(){
	if(!game)game=Game::currentGame();
	gameCamera.size=game->resolution;
}
GameScene::~GameScene(){}