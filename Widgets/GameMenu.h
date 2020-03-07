#ifndef GAMEMENU_H
#define GAMEMENU_H

#include"GameButton.h"

//菜单项,继承GameButton是为了使用其点击功能
class GameMenuItem:public GameButton{
public:
	GameMenuItem();
	~GameMenuItem();

	virtual void updateData(SizeType pos);//更新数据(pos为实际数据的位置,而不是item的位置)
	virtual void setSelected(bool b);//设置选中状态
};

/*游戏中用到的菜单,可以显示各种数据,本类提供一种比较简单的实现方法
注:本类为了能自动控制显示的列表项,子物体只容纳GameMenuItem和GameMenu的选择器
*/
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
	//选择器精灵,但并非所有风格的菜单都具有此精灵,根据需要自行设定
	GameSprite *pSpriteSelector;

	//构造函数
	GameMenu();
	virtual ~GameMenu();
	//光标移动
	void cursorMoveUp();
	void cursorMoveDown();

	virtual void updateRenderParameters(bool forceUpdate=false);//更新渲染参数
	virtual SizeType itemAmount()const;//菜单项数,可以大于实际显示数量renderItemAmount

	//菜单事件
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
	virtual bool mouseKey(MouseKey key, bool pressed);//鼠标点击菜单以外的区域则关闭菜单
	virtual bool mouseWheel(int angle);//鼠标滚轮可以进行选定
	virtual void onItemClicked(GameMenuItem *menuItem);//菜单项点击事件,用于响应菜单项被点击的情况
protected:
	//子类差异化函数
	virtual void addItem()=0;//添加Item,菜单项数变大时调用此函数进行调整
	virtual void removeItem()=0;//删除Item,菜单项数变小时候调用此函数进行调整
	virtual void updateItemsData();//更新每个Item的数据
	virtual void updateSelectCursor();//更新选择用的光标状态
	virtual void updateSize();//更新尺寸(根据菜单的当前状态)
	virtual void updateItemsPos();//更新每个菜单项的位置
};

//菜单模板,用于容纳T类型的菜单项,T类型一般为GameMenuItem的子类
//注:T类型需要带有updateData(SizeType pos)和setSelected(bool b),具体请看updateItemsData()和updateSelectCursor()
template<typename T>
class GameMenuTemplate:public GameMenu{
protected:
	T* itemArray;
public:
	GameMenuTemplate():itemArray(nullptr){}
	virtual ~GameMenuTemplate(){
		if(itemArray)delete []itemArray;
		deleteSubObject(pSpriteSelector);
	}

	//设置渲染的Item数量,立刻生成Item数组
	void setRenderItemAmount(SizeType amount){
		if(amount && amount!=renderItemAmount){
			renderItemAmount=amount;
			itemArray=new T[amount];
		}
	}
protected:
	void addItem(){addSubObject(&itemArray[subObjects.size()]);}
	void removeItem(){removeSubObject(&itemArray[subObjects.size()-1]);}
	void updateItemsData(){
		auto amount=itemAmount();
		amount=min(amount,renderItemAmount);
		for(SizeType i=0;i<amount;++i){
			itemArray[i].updateData(renderItemStart+i);
		}
	}
	void updateSelectCursor(){
		GameMenu::updateSelectCursor();
		auto amount=itemAmount();
		amount=min(amount,renderItemAmount);
		for(SizeType i=0;i<amount;++i){
			itemArray[i].setSelected(renderItemStart+i==selectingItemIndex);
		}
	}
};

//自带有图标和文字功能的条目
class GameMenuItem_IconName:public GameMenuItem{
protected:
	GameSprite spriteIcon;//条目图标
	GameString stringName;//条目名称
public:
	GameMenuItem_IconName();

	void setCursorWidth(SizeType cursorWidth);//设置光标宽度,成员控件会预留空间给光标
};
#endif