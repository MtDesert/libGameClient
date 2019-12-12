#ifndef GAMELAYER_CONVERSATION_H
#define GAMELAYER_CONVERSATION_H

#include"GameLayer.h"
#include"GameText.h"

/*对话界面,在大多数游戏中,对话可以出现在各种场合,比如特定的剧情对话,教程引导等*/
class GameLayer_Conversation:public GameLayer{
public:
	GameLayer_Conversation();
	~GameLayer_Conversation();

	//成员
	GameSprite dialogBox;//对话框
	GameText dialogText;//对话框的文本
	GameSprite bodySprite;//身体形象
	bool pausePrintText;//是否暂停显示对话

	void setDialogText(const char* text);//设置对话框文本,并做好显示的准备
	void setBodyImage(const string &filename);//设置身体形象
	//重写
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);
	virtual void consumeTimeSlice();//根据时间片,对文字进行逐个显示
};
#endif