#include "GameDialog_Message.h"
#include"Game.h"

const int border=16;
uint GameDialog_Message::maxLineCharAmount = 36;

GameDialog_Message::GameDialog_Message(){
	addSubObject(&mGameText);
	addSubObject(&mGameButton);
	mGameButton.setString(Game::currentGame()->translate("Confirm"));
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
	//调整mGameText的行字数
	auto strWidth = mGameText.stringWidth();
	if(strWidth > maxLineCharAmount * mGameText.charSize.x){
		mGameText.lineCharAmount = maxLineCharAmount;
	}else{
		mGameText.lineCharAmount = strWidth / mGameText.charSize.x;
	}
	mGameText.updateRenderParameter();
	//设置mGameText的几何位置
	auto rct=rectF();
	mGameText.position.y=rct.p1.y - border - mGameText.sizeF().y/2;
	//设置mGameButton的几何位置
	mGameButton.position.y=rct.p0.y + border + mGameButton.sizeF().y/2;
}