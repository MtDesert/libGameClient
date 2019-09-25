#include "GameScene_FileList.h"
#include"Game.h"
#include"extern.h"

GameScene_FileList::GameScene_FileList(){
	auto &res=Game::resolution;
	//标题,顶部居中
	textTitle.position.y=res.y/2;
	textTitle.anchorPoint.y=1;
	addSubObject(&textTitle);
	//路径
	textPath.position = textTitle.position;
	textPath.position.y -= textTitle.charSize.y;
	textPath.anchorPoint.y=1;
	textPath.setString(".");
	addSubObject(&textPath);
	//文件列表
	addSubObject(&gameTableDir);

	//按钮
	auto game=Game::currentGame();
	const char* names[]={"Confirm","File","Directory","Cancel"};
	GameButton_String *buttons[]={&buttonConfirm,&buttonFile,&buttonDir,&buttonCancel};
	auto btnWidth=res.x/4;
	for(auto i=0;i<4;++i){
		auto btn=buttons[i];
		btn->setString(game->translate(names[i]));
		btn->position.x = btnWidth*i - btnWidth*3/2;
		btn->position.y = -res.y/2 + btn->size.y/2;
		addSubObject(btn);
	}
}

bool GameScene_FileList::changeDirectory(const string &dirName){
	if(gameTableDir.changeDir(dirName)){
		textPath.setString(gameTableDir.directory.toString());
	}
	return false;
}
void GameScene_FileList::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	GameScene::keyboardKey(key,pressed);
	if(key==Keyboard::Key_Enter && !pressed){
		auto entry=gameTableDir.selectingDirectoryEntry();
		if(entry){
			if(entry->isDirectory()){//是目录,进行切换
				changeDirectory(entry->d_name);
			}else if(entry->isRegularFile()){//常规文件
				if(whenConfirmFile){
					whenConfirmFile(gameTableDir.directory.toString()+"/"+entry->d_name);
				}
			}
		}
	}
}