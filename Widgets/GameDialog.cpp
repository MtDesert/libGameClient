#include"GameDialog.h"
#include"Game.h"

GameDialog::GameDialog(){
	bgColor=&ColorRGBA::Black;
	borderColor=&ColorRGBA::White;
	forceIntercept=true;//对话框一般来说需要关闭掉,才让操作事件往下传递
}
GameDialog::~GameDialog(){}

void GameDialog::allSubObjects_verticalLayout(SizeType spacing){
	//计算宽度
	int maxWidth=0;
	forEachSubObj<GameSprite>([&](GameSprite &sprite){maxWidth = max(maxWidth,sprite.size.x);});
	size.x = maxWidth + spacing * 2;
	//计算高度
	size.y = spacing * (subObjects.size()+1);
	forEachSubObj<GameSprite>([&](GameSprite &sprite){size.y += sprite.size.y;});
	//调整控件位置
	int y=size.y/2;
	forEachSubObj<GameSprite>([&](GameSprite &sprite){
		y -= (spacing + sprite.size.y/2);
		sprite.position.y = y;
		y -= sprite.size.y/2;
	});
}
