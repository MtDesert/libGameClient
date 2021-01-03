#include"GameLayer_Conversation.h"
#include"Game.h"

BodyImage::BodyImage(){anchorPoint.y=0;}
GameLayer_Conversation::GameLayer_Conversation():pausePrintText(false){
	//人物身体层
	addSubObject(&objBodies);
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
	//自身大小
	size.setXY(Game::resolution.x,Game::resolution.y);
	//添加逐字显示功能
	Game::currentGame()->timeSliceList.pushTimeSlice(this,20,40);
}
GameLayer_Conversation::~GameLayer_Conversation(){
	Game::currentGame()->timeSliceList.removeTimeSlice(this);
}

void BodyImage::standAt(int pos){
	int w=Game::resolution.x,x=0;
	const int fix=32;
	switch(pos){
		case Left:x=-w/2 + fix*2;break;
		case MiddleLeft:x=-w/4 + fix;break;
		case Middle:x=0;break;
		case MiddleRight:x=w/4 - fix;break;
		case Right:x=w/2 - fix*2;break;
		default:;
	}
	position.x=x;
}
void BodyImage::faceTo(int dir){
	switch(dir){
		case Left:scale.x=1;break;
		case Right:scale.x=-1;break;
		default:;
	}
}
void BodyImage::hide(){
	name.clear();
	removeFromParentObject();
}

Texture *GameLayer_Conversation::bodyImage_str2tex(const char *name)const{return nullptr;}
void GameLayer_Conversation::setDialogText(const char *text){
	dialogText.setString(text);
	dialogText.readyPrint();
	pausePrintText=false;
}
BodyImage* GameLayer_Conversation::showBodyImage(const char* name){
	//先找出目前没使用的身体形象
	BodyImage *img=nullptr;
	for(int i=0;i<MAX_BODYIMAGE_AMOUNT;++i){
		if(bodyImages[i].name.empty()){
			img=&bodyImages[i];
			break;
		}
	}
	//找到了,赋值
	if(img){
		img->name=name;
		img->setTexture(bodyImage_str2tex(name));
		img->position.y=-Game::resolution.y/2;
		objBodies.reAddSubObject(img);
	}
	return img;
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
bool GameLayer_Conversation::mouseKey(MouseKey key,bool pressed){
	if(key==Mouse_LeftButton || !pressed){
		keyboardKey(Keyboard::Key_Enter,false);
		return true;
	}
	return false;
}
void GameLayer_Conversation::consumeTimeSlice(){
	//逐个显示字符
	if(pausePrintText)return;
	pausePrintText=dialogText.printNextChar();
}