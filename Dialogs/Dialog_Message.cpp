#include "Dialog_Message.h"
#include"Game.h"

const int border=16;
uint Dialog_Message::maxLineCharAmount = 36;

Dialog_Message::Dialog_Message(){
	addSubObject(&mGameText);
	addSubObject(&buttonsConfirmCancel);
	mGameText.setLineCharAmount(maxLineCharAmount,maxLineCharAmount);
	buttonsConfirmCancel.buttonConfirm.onClicked=[&](){//确定按钮
		removeFromParentObject();
	};
	buttonsConfirmCancel.buttonCancel.onClicked=[&](){//取消按钮
		removeFromParentObject();
	};
}
Dialog_Message::~Dialog_Message(){}

void Dialog_Message::setText(const string &text){
	mGameText.setString(text);
	size.x = mGameText.size.x + border*2;
	allSubObjects_verticalLayout(border);
}
void Dialog_Message::setConfirmCallback(GameButton::ClickCallback callback){
	buttonsConfirmCancel.buttonConfirm.onClicked=callback;
}