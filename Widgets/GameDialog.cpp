#include"GameDialog.h"
#include"Game.h"
#include"extern.h"

static ColorRGBA edgeColor(0xFFFFFFFF),fillColor(0xFF000000);

GameDialog::GameDialog(){}
GameDialog::~GameDialog(){}

void GameDialog::renderX()const{
	//画底板
	renderRect(&edgeColor,&fillColor);
}