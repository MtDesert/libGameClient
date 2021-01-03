#ifndef GAMELAYER_CONVERSATION_H
#define GAMELAYER_CONVERSATION_H

#include"GameLayer.h"
#include"GameText.h"

#define MAX_BODYIMAGE_AMOUNT 5

//对话层专用的身体形象
struct BodyImage:public GameSprite{
	BodyImage();

	enum StandPos{//身体形象的站位
		Left,MiddleLeft,Middle,MiddleRight,Right,
		AmountOfStandPos
	};
	string name;//名字,用于表明当前的身体形象
	void standAt(int pos);//站在特定位置(StandPos)
	void faceTo(int dir);//脸朝特定方向(StandPos)
	void hide();//身体隐藏(消失)
};

/*对话界面,在大多数游戏中,对话可以出现在各种场合,比如特定的剧情对话,教程引导等*/
class GameLayer_Conversation:public GameLayer{
	GameObject objBodies;//容纳身体形象的层
	BodyImage bodyImages[MAX_BODYIMAGE_AMOUNT];//身体形象,数量有限制
protected:
	virtual Texture* bodyImage_str2tex(const char *name)const;//形象名转纹理的规则,子类实现
public:
	GameLayer_Conversation();
	~GameLayer_Conversation();

	//成员
	GameSprite dialogBox;//对话框
	GameText dialogText;//对话框的文本
	bool pausePrintText;//是否暂停显示对话

	void setDialogText(const char* text);//设置对话框文本,并做好显示的准备
	BodyImage* showBodyImage(const char* name);//设置对话者身体形象
	//重写
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);
	virtual bool mouseKey(MouseKey key,bool pressed);
	virtual void consumeTimeSlice();//根据时间片,对文字进行逐个显示
};
#endif