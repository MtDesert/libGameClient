#include"GameLayer_Conversation.h"
#include"Game.h"

GameLayer_Conversation::GameLayer_Conversation():pausePrintText(false){
	//身体形象
	addSubObject(&bodySprite);
	//对话框,出现在屏幕底部
	dialogBox.anchorPoint.y=0;
	dialogBox.position.y=-Game::resolution.y/2;
	dialogBox.size.setXY(Game::resolution.x,dialogText.charSize.y*3);
	dialogBox.bgColor=&ColorRGBA::Black;
	dialogBox.borderColor=&ColorRGBA::White;
	addSubObject(&dialogBox);
	//对话文本,在对话框中显示文本内容
	dialogText.position.setXY(-dialogBox.size.x/2,dialogBox.size.y);
	dialogText.anchorPoint.setXY(0,1);
	dialogText.setLineCharAmount(3,dialogBox.size.x/dialogText.charSize.x);
	dialogBox.addSubObject(&dialogText);
	//添加逐字显示功能
	Game::currentGame()->timeSliceList.pushTimeSlice(this,20,40);
}
GameLayer_Conversation::~GameLayer_Conversation(){
	Game::currentGame()->timeSliceList.removeTimeSlice(this);
}

void GameLayer_Conversation::setDialogText(const char *text){
	dialogText.setString(text);
	dialogText.readyPrint();
	pausePrintText=false;
}
void GameLayer_Conversation::setBodyImage(const string &filename){
	bodySprite.texture.texImage2D_FileName(filename);
}

bool GameLayer_Conversation::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	bool ret=false;
	if(key==Keyboard::Key_Enter && !pressed){
		if(pausePrintText){//显示下一行对话
			pausePrintText=false;
			if(dialogText.printNextLine());
			else{
				auto script=Game::currentGame()->scenarioScript;
				if(script)script->scriptContinue();
			}
		}
		ret=true;
	}
	return ret;
}
void GameLayer_Conversation::consumeTimeSlice(){
	//逐个显示字符
	if(pausePrintText)return;
	pausePrintText=dialogText.printNextChar();
}