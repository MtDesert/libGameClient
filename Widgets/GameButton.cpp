#include"GameButton.h"
#include"Game.h"
#include"extern.h"

static ColorRGBA borderColor(0xFFFFFFFF);

GameButton::GameButton():isPressed(false),bgColor(0xFF000000),onClicked(nullptr){
	size.x=160;
	size.y=32;
}
GameButton::~GameButton(){}
GameButton_String::GameButton_String(){
	addSubObject(&mGameString);
}
GameButton_String::~GameButton_String(){}

void GameButton::mouseMove(int x,int y){
	if(!isMouseOnButton()){
		setIsPressed(false);
	}
}
void GameButton::mouseKey(MouseKey key,bool pressed){
	if(isMouseOnButton()){
		bool changed=(isPressed!=pressed);
		setIsPressed(pressed);
		if(changed && !isPressed){
			if(onClicked)onClicked(this);
		}
	}
}
void GameButton::renderX()const{
	rect=rectF();
	//绘制按钮纹理
	shapeRenderer.hasFill=true;
	shapeRenderer.fillColor=(isPressed ? color : bgColor);
	shapeRenderer.drawRectangle(rect);
	//绘制按钮边框
	renderRect(&borderColor);
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

void GameButton_String::setString(const string &str){
	mGameString.setString(str);
}

void GameButton_String::setIsPressed(bool pressed){
	GameButton::setIsPressed(pressed);
	mGameString.color=(isPressed ? bgColor : color);
}