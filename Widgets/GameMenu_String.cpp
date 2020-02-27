#include"GameMenu_String.h"
#include"Game.h"

GameMenu_String::GameMenu_String(){borderColor=&ColorRGBA::White;}
GameMenu_String::~GameMenu_String(){}

void GameMenuItem_String::updateData(SizeType pos){
	auto menu=dynamic_cast<GameMenu_String*>(parentObject);
	if(menu){
		auto pStr=menu->gameStringList.data(pos);
		if(pStr){
			setRawString(*pStr);
		}
	}
}
void GameMenuItem_String::setSelected(bool b){
	color = b ? ColorRGBA::Black : ColorRGBA::White;
	bgColor = b ? &ColorRGBA::White : nullptr;
}

void GameMenu_String::addString(const string &str,bool translate){
	auto block=GameString::charset.newString(translate ? Game::currentGame()->translate(str) : str.data());
	gameStringList.push_back((char*)block.dataPointer);
}

SizeType GameMenu_String::rowAmount()const{return gameStringList.size();}