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
			if(onClicked)onClicked();
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
	size.setXY(max(size.x,mGameString.size.x),max(size.y,mGameString.size.y));//设置完成后,调整自身大小
}

void GameButton_String::setIsPressed(bool pressed){
	GameButton::setIsPressed(pressed);
	bgColor=&(isPressed ? ColorRGBA::White:ColorRGBA::Black);
	mGameString.color=(isPressed ? ColorRGBA::Black:ColorRGBA::White);
}

//Confirm,Cancel
GameButtonGroup_ConfirmCancel::GameButtonGroup_ConfirmCancel(){
	//设置文字
	buttonConfirm.setString("Confirm",true);
	buttonCancel.setString("Cancel",true);
	//设置控件尺寸位置
	const SizeType spacing=16;
	size.setXY(buttonConfirm.size.x + spacing + buttonCancel.size.x,max(buttonConfirm.size.y,buttonCancel.size.y));
	buttonConfirm.position.x = -size.x/2 + buttonConfirm.size.x/2;
	buttonCancel.position.x = -buttonConfirm.position.x;
	//添加
	addSubObject(&buttonConfirm);
	addSubObject(&buttonCancel);
}
void GameButtonGroup_ConfirmCancel::setConfirmCancelFunction(ClickCallback confirmCallback,ClickCallback cancelCallback){
	buttonConfirm.onClicked=confirmCallback;
	buttonCancel.onClicked=cancelCallback;
}