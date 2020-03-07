#ifndef GAMETABLE_DIR_H
#define GAMETABLE_DIR_H

#include"GameTable.h"
#include"Directory.h"

//文件表项控件
class GameTable_DirItem:public GameMenuItem{
public:
	GameTable_DirItem();

	GameString stringFileName;//文件名
	GameString stringFileSize;//文件大小

	void updateData(SizeType pos);
	void setSelected(bool b);//设置选中状态
};

/*显示目录内容的表格,有的时候我们需要在游戏中动态加载文件*/
class GameTable_Dir:public GameTableTemplate<GameTable_DirItem>{
public:
	GameTable_Dir();
	~GameTable_Dir();

	Directory directory;//保存当前的目录,以便提供数据显示文件
	bool changeDir(const string &dirName,WhenErrorString whenError=nullptr);//切换目录,并刷新视图
	const DirectoryEntry* selectingDirectoryEntry()const;

	//重写
	virtual SizeType itemAmount()const;
	virtual SizeType columnAmount()const;//显示文件的属性列数
	virtual SizeType columnWidth(SizeType column)const;//设定各个列宽
protected:
	virtual void updateSize();
};
#endif