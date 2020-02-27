#include"GameDialog.h"
#include"Game.h"
#include"extern.h"

GameDialog::GameDialog(){
	bgColor=&ColorRGBA::Black;
	borderColor=&ColorRGBA::White;
}
GameDialog::~GameDialog(){}

bool GameDialog::keyboardKey(Keyboard::KeyboardKey key,bool pressed){return true;}

void GameDialog::allSubObjects_verticalLayout(SizeType spacing){
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