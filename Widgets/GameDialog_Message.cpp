#include "GameDialog_Message.h"
#include"Game.h"

const int border=16;
uint GameDialog_Message::maxLineCharAmount = 36;

GameDialog_Message::GameDialog_Message(){
	addSubObject(&mGameText);
	addSubObject(&mGameButton);
	mGameText.setLineCharAmount(maxLineCharAmount,maxLineCharAmount);
	mGameButton.setString("Confirm",true);
	mGameButton.onClicked=[](){//按下按钮后清除消息
		Game::hideDialogMessage();
		Game::clearErrorMessages();
	};
}
GameDialog_Message::~GameDialog_Message(){}

void GameDialog_Message::setText(const string &text){
	mGameText.setString(text);
	//计算高度
	size.setXY(mGameText.size.x + border*2,mGameText.size.y + mGameButton.size.y + border*3);
	auto halfHeight = size.y/2;
	mGameText.position.y = halfHeight - border - mGameText.size.y/2;//设置mGameText的几何位置
	mGameButton.position.y= -halfHeight + border + mGameButton.size.y/2;//设置mGameButton的几何位置
}