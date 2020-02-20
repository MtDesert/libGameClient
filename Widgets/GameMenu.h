#ifndef GAMEMENU_H
#define GAMEMENU_H

#include"GameSprite.h"
#include"List.h"
#include"ShapeRenderer.h"

/*游戏中用到的菜单,可以显示各种数据,具体的显示方法一般在子类实现*/
class GameMenu:public GameSprite{
public:
	//渲染变量,修改此值会直接改变渲染结果
	SizeType renderItemStart;//渲染开始项,表示从第几项开始渲染
	SizeType renderItemAmount;//最多显示多少项,多余的项不会被绘制
	//菜单状态
	SizeType selectingItemIndex;//当前选择的索引
	bool recycleMode;//循环模式,末项的下一项是首项,首项的上一项是末项
	//回调函数
	function<void(GameMenu*)>onConfirm;
	function<void(GameMenu*)>onCancel;

	//构造函数
	GameMenu();
	~GameMenu();

	virtual void updateRenderParameters();//更新渲染参数
	virtual SizeType rowAmount()const;//菜单项数,可以大于实际显示数量renderItemAmount

	//override
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
protected:
	//子类差异化函数
	virtual void addItem();//添加Item,菜单项数变大时调用此函数进行调整
	virtual void removeItem();//删除Item,菜单项数变小时候调用此函数进行调整
	virtual void updateItemsData();//更新每个Item的数据
	virtual void updateSelectCursor();//更新选择用的光标状态
	virtual void updateSize();//更新尺寸(根据菜单的当前状态)
	virtual void updateItemsPos();//更新每个菜单项的位置
};
#endif