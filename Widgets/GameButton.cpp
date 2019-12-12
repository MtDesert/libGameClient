#include"GameButton.h"
#include"Game.h"
#include"extern.h"

GameButton::GameButton():isPressed(false),onClicked(nullptr){
	size.setXY(160,32);
	bgColor=&ColorRGBA::Black;
	borderColor=&ColorRGBA::White;
}
GameButton::~GameButton(){}
GameButton_String::GameButton_String(){
	addSubObject(&mGameString);
}
GameButton_String::~GameButton_String(){}

bool GameButton::mouseMove(int x,int y){
	if(!isMouseOnButton()){
		setIsPressed(false);
		return true;
	}
	return false;
}
bool GameButton::mouseKey(MouseKey key,bool pressed){
	if(isMouseOnButton()){
		bool changed=(isPressed!=pressed);
		setIsPressed(pressed);
		if(changed && !isPressed){
			if(onClicked)onClicked(this);
		}
		return true;
	}
	return false;
}

bool GameButton::isMouseOnButton()const{
	rect=rectF();
	auto &pos(Game::currentGame()->mousePos);
	//判断前需要进行平移
	rect.translate(position.x,position.y);
	auto parent=this->parentObject;
	while(parent){
		auto sprite=dynamic_cast<GameSprite*>(parent);
		if(sprite){
			rect.translate(sprite->position.x,sprite->position.y);
			parent=sprite->parentObject;
		}else{
			parent=nullptr;
		}
	}
	return rect.containPoint(pos.x,pos.y);
}
void GameButton::setIsPressed(bool pressed){isPressed=pressed;}

void GameButton_String::setString(const string &str,bool translate){
	mGameString.setString(str,translate);
}

void GameButton_String::setIsPressed(bool pressed){
	GameButton::setIsPressed(pressed);
	bgColor=&(isPressed ? ColorRGBA::White:ColorRGBA::Black);
	mGameString.color=(isPressed ? ColorRGBA::Black:ColorRGBA::White);
}