#ifndef GAMESTRINGINPUTBOX_H
#define GAMESTRINGINPUTBOX_H

#include"GameButton.h"

//游戏输入框,让玩家通过系统提供的输入功能来进行数据输入
class GameInputBox:public GameButton_String{
public:
	GameInputBox();
	function<void()> whenInputConfirm;//当输入确认后,调用此函数(如果有的话)

	static void startInput(GameInputBox *inputBox);//开始输入,一般通过具体的系统调用来解决
	static void inputing();//输入回调,由Game频繁调度
	static void finishInput();
};
//布尔输入框,只能输入两种值
class GameInputBox_Bool:public GameInputBox{
public:
	GameInputBox_Bool();

	bool boolValue;
	const char *falseStr,*trueStr;//真假时需要显示的内容
	void setValue(bool b);
};
//字符串输入框,非常常见的一种框
class GameInputBox_String:public GameInputBox{
public:
	GameInputBox_String();

	string mString;//字符串内容,可直接读取此变量的值,写时请用setString()
	char passwordChar;//密码字符,'\0'为正常显示,一般密码可以设置为'*',当然也可以设置成你想要的其它字符
	void setValue(const string &str);
};
//整数输入框
class GameInputBox_Integer:public GameInputBox{
public:
	GameInputBox_Integer();

	int mInteger;//整数内容,获取整数值,写时请用setInteger
	int minInteger,maxInteger;//值域
	void setValue(int num);
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

	void setLabelName_ValueWidth_MaxWidth(const string &labelName,bool translate,SizeType valueWidth,SizeType maxWidth){
		//设置标签
		stringLabel.position.x=-maxWidth/2;
		stringLabel.anchorPoint.x=0;
		stringLabel.setString(labelName,translate);
		//设置输入框
		inputBox.size.setXY(valueWidth,stringLabel.size.y);
		inputBox.position.x = maxWidth/2 - inputBox.size.x/2;
		size.setXY(maxWidth,stringLabel.size.y);//调整本对象大小
	}
};

//各种输入框
class GameAttr_InputBoxBool:public GameAttr_InputBox<GameInputBox_Bool>{
public:
	bool getValue()const;
};
class GameAttr_InputBoxString:public GameAttr_InputBox<GameInputBox_String>{
public:
	string getValue()const;
};
class GameAttr_InputBoxInteger:public GameAttr_InputBox<GameInputBox_Integer>{
public:
	int getValue()const;//获取整数(可能有负数)
	unsigned getUnsignedValue()const;//获取无符号数
};
#endif