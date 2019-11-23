#include "GameDialog_Message.h"
#include"Game.h"

const int border=16;
uint GameDialog_Message::maxLineCharAmount = 36;
static void whenConfirmClicked(GameButton *button){
	Game::hideDialogMessage();
}

GameDialog_Message::GameDialog_Message(){
	addSubObject(&mGameText);
	addSubObject(&mGameButton);
	mGameText.setLineCharAmount(maxLineCharAmount,maxLineCharAmount);
	mGameButton.setString("Confirm",true);
	mGameButton.onClicked=whenConfirmClicked;
}
GameDialog_Message::~GameDialog_Message(){}

Point2D<float> GameDialog_Message::sizeF()const{
	Point2D<float> ret;
	auto textRect=mGameText.rectF();
	ret.x=textRect.width() + border*2;
	ret.y=textRect.height() + mGameButton.sizeF().y+border*3;
	return ret;
}
void GameDialog_Message::setText(const string &text){
	mGameText.setString(text);
	//设置mGameText的几何位置
	auto rct=rectF();
	mGameText.position.y=rct.p1.y - border - mGameText.sizeF().y/2;
	//设置mGameButton的几何位置
	mGameButton.position.y=rct.p0.y + border + mGameButton.sizeF().y/2;
}