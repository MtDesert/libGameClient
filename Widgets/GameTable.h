#ifndef GAMETABLE_H
#define GAMETABLE_H

#include"GameMenu.h"
#include"GameString.h"

/*游戏中常用的表,表的作用是用来给玩家查询一些数据
本类只负责简单画上表格线,以及一些文字数据,不负责存储数据
*/
class GameTable:public GameMenu{
public:
	//变量
	SizeType itemHeight;//菜单项的寬高
	SizeType page,maxPage;//当前页,最大页数(某些游戏数据庞大,一页是显示不完的)

	//构造/析构函数
	GameTable();
	~GameTable();

	//函数
	virtual uint columnAmount()const;//列数,间接决定了尺寸
	virtual uint columnWidth(uint column)const;//各个列的宽度,在子类中实现
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);
	//渲染
	virtual void renderX()const;
};
#endif