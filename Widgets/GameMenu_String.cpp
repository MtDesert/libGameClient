#include"GameMenu_String.h"
#include"Game.h"

GameMenuItem_String::GameMenuItem_String(){addSubObject(&gameString);}
GameMenuItem_String::~GameMenuItem_String(){}
GameMenu_String::GameMenu_String(){}
GameMenu_String::~GameMenu_String(){}

void GameMenuItem_String::updateData(SizeType pos){
	auto menu=dynamic_cast<GameMenu_String*>(parentObject);
	if(menu){
		auto pStr=menu->gameStringList.data(pos);
		if(pStr){
			gameString.setRawString(*pStr);
		}
	}
}
void GameMenuItem_String::setSelected(bool b){
	color = b ? ColorRGBA::Black : ColorRGBA::White;
	bgColor = b ? &ColorRGBA::White : nullptr;
	gameString.color = b ? ColorRGBA::Black : ColorRGBA::White;
}

void GameMenu_String::addString(const string &str,bool translate){
	auto block=GameString::charset.newString(translate ? Game::currentGame()->translate(str) : str.data());
	gameStringList.push_back((char*)block.dataPointer);
}

SizeType GameMenu_String::itemAmount()const{return gameStringList.size();}