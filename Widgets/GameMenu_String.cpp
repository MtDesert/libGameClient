#include"GameMenu_String.h"
#include"Game.h"
#include"extern.h"

GameMenu_String::GameMenu_String(){borderColor=&ColorRGBA::White;}
GameMenu_String::~GameMenu_String(){}

void GameMenu_String::addString(const string &str,bool translate){
	auto block=GameString::charset.newString(translate ? Game::currentGame()->translate(str) : str.data());
	gameStringList.push_back((char*)block.dataPointer);
}

SizeType GameMenu_String::rowAmount()const{return gameStringList.size();}
void GameMenu_String::addItem(){addSubObject(new GameString());}
void GameMenu_String::updateItemsData(){
	auto pos=renderItemStart;
	forEachSubObj<GameString>([&](GameString *gameString){
		auto pStr=gameStringList.data(pos);
		if(pStr){
			gameString->setRawString(*pStr);
		}
		++pos;
	});
}
void GameMenu_String::updateSelectCursor(){
	auto pos=renderItemStart;
	forEachSubObj<GameString>([&](GameString *gameString){
		auto pStr=gameStringList.data(pos);
		if(pStr){
			//修改样式
			if(pos==selectingItemIndex){//选中态,白底黑字
				gameString->color=ColorRGBA::Black;
				gameString->bgColor=&ColorRGBA::White;
			}else{//非选中态,无底白字
				gameString->color=ColorRGBA::White;
				gameString->bgColor=nullptr;
			}
		}
		++pos;
	});
}