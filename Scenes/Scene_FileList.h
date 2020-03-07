#ifndef SCENE_FILELIST_H
#define SCENE_FILELIST_H

#include"GameScene.h"
#include"GameTable_Dir.h"
#include"GameButton.h"
#include"GameInputBox.h"

#include"Directory.h"

/*选择文件用的场景*/
class Scene_FileList:public GameScene{
public:
	Scene_FileList();

	virtual void reset();//重置,清空目录
	//更换目录
	bool changeDirectory(const string &dirName);
	//设置打开或者保存模式,saveMode为true时候为保存模式,否则为打开模式
	void setSaveMode(bool saveMode);
	bool isSaveMode()const;

	GameString stringTitle;//标题,告知玩家需要选择什么文件
	GameString stringPath;//路径,用于显示文件位置
	GameTable_Dir gameTableDir;//文件列表,用于显示文件名等基本信息

	GameAttr_InputBoxString attrSaveFilename;//保存的文件名,用于保存文件时候输入

	GameButton_String buttonConfirm;//确认按钮,用来打开文件
	GameButton_String buttonFile;//文件按钮,对选中文件进行操作
	GameButton_String buttonDir;//目录按钮,使用目录的常用功能
	GameButton_String buttonCancel;//取消按钮,关闭本页面

	GameScene *lastScene;//上一个场景,即进入本场景之前的场景

	function<void(const string &filename)>whenConfirmFile;//当确定选择的文件后,调用此回调函数,传递选择的文件相对路径到filename

	void selectFile(bool saveMode,const string &title,const string& path,decltype(whenConfirmFile) callback);
};
#endif