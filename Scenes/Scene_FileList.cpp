#include "Scene_FileList.h"
#include"Game.h"
#include"extern.h"

#include<unistd.h>

Scene_FileList::Scene_FileList():lastScene(nullptr){
	auto &res=Game::resolution;
	//标题,顶部居中
	stringTitle.position.y=res.y/2;
	stringTitle.anchorPoint.y=1;
	addSubObject(&stringTitle);
	//路径
	stringPath.position = stringTitle.position;
	stringPath.position.y -= stringTitle.charSize.y;
	stringPath.anchorPoint.y=1;
	stringPath.setString(".");
	addSubObject(&stringPath);
	//文件列表
	gameTableDir.position.y += 16;
	addSubObject(&gameTableDir);
	//保存文件名
	attrSaveFilename.setLabel_Spacing_MaxWidth("Filename",true,16,600);
	attrSaveFilename.position.y = -res.y/2 + attrSaveFilename.size.y*2;
	//按钮
	const char* names[]={"Confirm","File","Directory","Cancel"};
	GameButton_String *buttons[]={&buttonConfirm,&buttonFile,&buttonDir,&buttonCancel};
	auto btnWidth=res.x/4;
	for(auto i=0;i<4;++i){
		auto btn=buttons[i];
		btn->setString(names[i],true);
		btn->position.x = btnWidth*i - btnWidth*3/2;
		btn->position.y = -res.y/2 + btn->size.y/2;
		addSubObject(btn);
	}

	//设置按钮事件
	buttonConfirm.onClicked=[&](){//确定按钮,切换目录或者打开文件
		auto entry=gameTableDir.selectingDirectoryEntry();
		if(entry){
			string fullPath="";
			if(isSaveMode() && entry->name()=="."){//保存模式下,以attrSaveFilename上的名字来命名
				fullPath = gameTableDir.directory.toString()+"/"+attrSaveFilename.getValue();
			}else if(entry->isDirectory()){//是目录,进行切换
				changeDirectory(entry->d_name);
			}else if(entry->isRegularFile()){//常规文件
				fullPath = gameTableDir.directory.toString()+"/"+entry->d_name;
			}
			//文件操作
			if(!fullPath.empty()){
				//判断是否需要询问
				auto needAskOverWrite=false;
				if(isSaveMode()){//保存文件,要对覆盖情况进行提示
					if(entry->name()=="." && attrSaveFilename.getValue().empty())return;//没写文件名
					needAskOverWrite=(access(fullPath.data(),F_OK)==0);
				}
				//询问或者打开/保存文件
				if(needAskOverWrite){
					auto dialog=Game::currentGame()->showDialog_Message();
					dialog->setText(fullPath+" is exist, overwrite?");
					dialog->setConfirmCallback([&,fullPath](){
						if(whenConfirmFile)whenConfirmFile(fullPath);
					});
				}else{
					if(whenConfirmFile)whenConfirmFile(fullPath);
				}
			}
		}
	};
	buttonCancel.onClicked=[&](){//取消按钮,返回到之前跳转进来的场景
		if(lastScene){
			Game::currentGame()->gotoScene(*lastScene);
		}else{
			removeFromParentObject();
		}
	};
	//菜单事件和确定按钮事件相同
	gameTableDir.onConfirm=[&](GameMenu*){
		buttonConfirm.onClicked();
	};
}

void Scene_FileList::reset(){
	GameScene::reset();
	stringTitle.setString("");
	gameTableDir.directory.clear();
	lastScene=nullptr;
	whenConfirmFile=nullptr;
}
bool Scene_FileList::changeDirectory(const string &dirName){
	if(gameTableDir.changeDir(dirName,Game::whenError)){
		stringPath.setString(gameTableDir.directory.toString());
	}else if(dirName!="."){
		changeDirectory(".");//尝试读取程序所在目录
	}
	return false;
}
void Scene_FileList::setSaveMode(bool saveMode){
	removeSubObject(&attrSaveFilename);
	if(saveMode){
		addSubObject(&attrSaveFilename);
	}
}
bool Scene_FileList::isSaveMode()const{return attrSaveFilename.parentObject==this;}