#ifndef GAMESTRINGINPUTBOX_H
#define GAMESTRINGINPUTBOX_H

#include"GameButton.h"

//游戏输入框,让玩家通过系统提供的输入功能来进行数据输入
class GameInputBox:public GameButton_String{
public:
	GameInputBox();
	virtual void startInput()=0;//开始输入,一般通过具体的系统调用来解决

	static void updateInput();//更新当前输入框的内容,由Game调度
};
//字符串输入框,非常常见的一种框
class GameInputBox_String:public GameInputBox{
public:
	GameInputBox_String();

	string mString;//字符串内容,可直接读取此变量的值,写时请用setString()
	char passwordChar;//密码字符,'\0'为正常显示,一般密码可以设置为'*',当然也可以设置成你想要的其它字符
	void setString(const string &str);
	//override
	virtual void startInput();
};
//整数输入框
class GameInputBox_Integer:public GameInputBox{
public:
	GameInputBox_Integer();

	int mInteger;//整数内容,获取整数值,写时请用setInteger
	int minInteger,maxInteger;//值域
	void setInteger(int num);
	//override
	virtual void startInput();
};

//属性输入框控件,自带具有提示用的标签
template<typename InputBoxType>
class GameAttr_InputBox:public GameSprite{
public:
	GameString stringLabel;//标签名,用于指明这是什么属性
	InputBoxType inputBox;//输入框,保存输入内容

	GameAttr_InputBox(){
		addSubObject(&stringLabel);
		addSubObject(&inputBox);
	}

	void setLabel_Spacing_MaxWidth(const string &labelName,bool translate,SizeType spacing,SizeType maxWidth){
		//设置标签
		stringLabel.position.x=-maxWidth/2;
		stringLabel.anchorPoint.x=0;
		stringLabel.setString(labelName,translate);
		//设置输入框
		size.setXY(maxWidth,max(stringLabel.size.y,inputBox.size.y));//调整本对象大小
		inputBox.size.x = maxWidth - spacing - stringLabel.size.x; //动态计算inputBox大小
		inputBox.position.x = maxWidth/2 - inputBox.size.x/2;
	}
};

class GameAttr_InputBoxString:public GameAttr_InputBox<GameInputBox_String>{
public:
	string getValue()const;
};
class GameAttr_InputBoxInteger:public GameAttr_InputBox<GameInputBox_Integer>{
public:
	int getValue()const;
};

#endif