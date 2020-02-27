#ifndef GAMEMENU_STRING_H
#define GAMEMENU_STRING_H

#include"GameMenu.h"
#include"GameString.h"

//文字菜单项
class GameMenuItem_String:public GameString{
public:
	void updateData(SizeType pos);
	void setSelected(bool b);
};

/*只显示文字项的菜单,实现方法比较简单,子类可以参考这个例子*/
class GameMenu_String:public GameMenuTemplate<GameMenuItem_String>{
public:
	//构造/析构函数
	GameMenu_String();
	~GameMenu_String();

	//字符串
	List<string> gameStringList;//保存解码的内容
	void addString(const string &str,bool translate=false);//添加字符串

	//重写
	virtual SizeType rowAmount()const;
};
#endif