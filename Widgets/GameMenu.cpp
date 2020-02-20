#include"GameMenu.h"
#include"extern.h"

//构造函数
GameMenu::GameMenu():
	renderItemStart(0),renderItemAmount(4),
	selectingItemIndex(0),recycleMode(true),
	onConfirm(nullptr),onCancel(nullptr){}
GameMenu::~GameMenu(){while(subObjects.size())removeItem();}

void GameMenu::updateRenderParameters(){
	//调整ITEM数量
	auto dataAmount = rowAmount();//数据总数
	auto renderAmount = min(dataAmount,renderItemAmount);//实际渲染数<=数据总数
	while(subObjects.size()<renderAmount)addItem();//添加
	while(subObjects.size()>renderAmount)removeItem();//删除
	//更新ITEM选择状态
	selectingItemIndex = min(selectingItemIndex,dataAmount);
	auto renderEnd = renderItemStart + renderAmount;
	if(renderEnd > dataAmount){//修正renderItemStart防止越界
		renderItemStart = dataAmount - renderAmount;
		renderEnd = dataAmount;
	}
	//更新item内容,更新选择控件
	if(selectingItemIndex < renderItemStart){
		renderItemStart = selectingItemIndex;//往前移动
	}else if(selectingItemIndex >= renderEnd){
		renderItemStart = selectingItemIndex - renderAmount + 1;//往后移动
	}
	updateItemsData();
	updateSelectCursor();
	updateSize();
	updateItemsPos();
}
//菜单项数
SizeType GameMenu::rowAmount()const{return subObjects.size();}

//菜单可以响应键盘事件(比如方向键选择,回车键确定,退出键关闭菜单等)
bool GameMenu::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	if(pressed)return false;
	bool ret=true;
	switch(key){
		//下键
		case Keyboard::Key_Down:
			++selectingItemIndex;
			if(selectingItemIndex>=rowAmount()){
				selectingItemIndex = recycleMode ? 0 : selectingItemIndex-1;
			}
			updateRenderParameters();
		break;
		//上键
		case Keyboard::Key_Up:
			if(selectingItemIndex>0){
				--selectingItemIndex;
			}else{
				selectingItemIndex = recycleMode ? rowAmount()-1 : 0;
			}
			updateRenderParameters();
		break;
		case Keyboard::Key_Enter:
			if(onConfirm)onConfirm(this);
		break;
		case Keyboard::Key_Esc:
			if(onCancel)onCancel(this);
		break;
		default:ret=false;
	}
	return ret;
}
void GameMenu::addItem(){addSubObject(new GameSprite());}
void GameMenu::removeItem(){deleteSubObject(*subObjects.lastData());}
void GameMenu::updateItemsData(){}
void GameMenu::updateSelectCursor(){}
void GameMenu::updateSize(){
	size.setXY(0,0);
	forEachSubObj<GameSprite>([&](GameSprite *sprite){
		size.x = max(size.x,sprite->size.x);
		size.y += sprite->size.y;
	});
}
void GameMenu::updateItemsPos(){
	//调整各个控件位置
	auto y=size.y/2;
	forEachSubObj<GameSprite>([&](GameSprite *sprite){
		sprite->position.y=y - sprite->size.y/2;
		y -= sprite->size.y;
	});
}