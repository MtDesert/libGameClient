#include"GameButton.h"
#include"Game.h"

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
	if(!isMouseOnSprite()){//鼠标移开按钮时候,取消其按下状态
		setIsPressed(false);
	}
	return false;
}
bool GameButton::mouseKey(MouseKey key,bool pressed){
	if(isMouseOnSprite()){//改变按钮按下状态,并检查时候要触发onClicked函数
		bool changed=(isPressed!=pressed);
		setIsPressed(pressed);
		if(changed && !isPressed){
			if(onClicked)onClicked();
		}
		return true;
	}
	return false;
}

void GameButton::setIsPressed(bool pressed){isPressed=pressed;}

void GameButton_String::setString(const string &str,bool translate){
	mGameString.setString(str,translate);
	size.setXY(max(size.x,mGameString.size.x),max(size.y,mGameString.size.y));//设置完成后,调整自身大小
}
void GameButton_String::resizeAsString(){size=mGameString.size;}
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
void GameButtonGroup_ConfirmCancel::showButtonCancel(bool show){
	if(show){//显示取消按钮
		reAddSubObject(&buttonCancel);
		const int spacing=16;
		horizontalLayout(-(buttonConfirm.size.x+spacing+buttonCancel.size.x)/2,spacing);
	}else{//隐藏取消按钮
		buttonCancel.removeFromParentObject();
		horizontalLayout(-buttonConfirm.size.x/2,0);
	}
}
void GameButtonGroup_ConfirmCancel::setConfirmCancelFunction(ClickCallback confirmCallback,ClickCallback cancelCallback){
	buttonConfirm.onClicked=confirmCallback;
	buttonCancel.onClicked=cancelCallback;
}