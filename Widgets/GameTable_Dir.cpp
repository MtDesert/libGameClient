#include"GameTable_Dir.h"
#include"extern.h"

enum ColumnType{
	FileName,FileSize,AmountOfColumnType
};

GameTable_Dir::GameTable_Dir(){
	//设定数据源
	renderItemAmount=10;
	itemHeight=32;
	fileInfoBuffer=new FileInfo[renderItemAmount];
	//关联控件
	rect=rectF();
	for(uint i=0;i<renderItemAmount;++i){
		auto &fileName=fileInfoBuffer[i].fileName;
		auto &fileSize=fileInfoBuffer[i].fileSize;
		//文件名位置
		fileName.anchorPoint.x=0;
		fileName.position.x=rect.left();
		fileName.position.y=rect.top() - fileName.sizeF().y * (i+0.5);
		addSubObject(&fileName);
		//文件大小位置
		fileSize.anchorPoint.x=1;
		fileSize.position.x=rect.right();
		fileSize.position.y=fileName.position.y;
		addSubObject(&fileSize);
	}
}
GameTable_Dir::~GameTable_Dir(){
	subObjects.clear();
	delete []fileInfoBuffer;//删除缓冲
}

bool GameTable_Dir::changeDir(const string &dirName){
	bool b=directory.changeDir(dirName);
	if(b){
		directory.readDir();
		directory.direntList.sortBy(DirentList::ByName);
		renderItemStart=selectingItemIndex=0;
		updateBuffer();
		updateSelecting();
	}
	return b;
}
const DirectoryEntry *GameTable_Dir::selectingDirectoryEntry()const{
	auto itr=getDirentItr(selectingItemIndex);
	return itr==directory.direntList.end() ? nullptr : &(*itr);
}

void GameTable_Dir::renderItem(uint x,uint y,const Rectangle2D<float> &rectArea)const{
	switch(x){
		case FileName:break;
		case FileSize:break;
	}
}

uint GameTable_Dir::rowAmount()const{return directory.direntList.size();}
uint GameTable_Dir::columnAmount()const{return 3;}
uint GameTable_Dir::columnWidth(uint col)const{
	switch(col){
		case FileName:return 400;
		case FileSize:return 120;
		default:return 0;
	}
}
bool GameTable_Dir::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	auto start=renderItemStart,sel=selectingItemIndex;
	bool ret=GameTable::keyboardKey(key,pressed);
	//改变状态,这会影响到渲染过程
	if(start!=renderItemStart){
		updateBuffer();
		ret=true;
	}
	if(sel!=selectingItemIndex){
		updateSelecting();
		ret=true;
	}
	return ret;
}

void GameTable_Dir::updateBuffer(){
	//更新迭代器
	auto itr=getDirentItr(renderItemStart);
	//更新缓冲
	for(decltype(renderItemAmount) i=0;i<renderItemAmount;++i){
		//更新显示
		auto &info=fileInfoBuffer[i];
		bool hasInfo=i<directory.direntList.size();
		info.fileName.setString(hasInfo ? itr->d_name : "");
		info.fileSize.setString(hasInfo ? itr->strSize() : "");
		//下一个
		++itr;
	}
}
void GameTable_Dir::updateSelecting(){
	auto index=selectingItemIndex-renderItemStart;
	auto black=ColorRGBA(0,0,0),white=ColorRGBA(0xFF,0xFF,0xFF);
	for(decltype(renderItemAmount) i=0;i<renderItemAmount;++i){
		if(i>=directory.direntList.size())break;//防止读取不存在的数据
		//更新显示
		auto &info=fileInfoBuffer[i];
		//更新当前选择项状态
		bool isSelected=(i==index);
		info.fileName.color=isSelected?black:white;
		info.fileSize.color=info.fileName.color;
	}
}
DirentList::const_iterator GameTable_Dir::getDirentItr(uint pos)const{
	auto itr=directory.direntList.begin();
	for(uint i=0;i<pos;++i){
		if(itr!=directory.direntList.end())++itr;
		else break;
	}
	return itr;
}