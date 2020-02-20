#include"GameTable_Dir.h"
#include"extern.h"

enum ColumnType{
	FileName,FileSize,AmountOfColumnType
};

GameTable_DirItem::GameTable_DirItem(){
	//尺寸
	stringFileName.size.x=400;
	stringFileSize.size.x=120;
	this->size.setXY(stringFileName.size.x+stringFileSize.size.x,stringFileName.charSize.y);
	//位置
	stringFileName.position.x=-this->size.x/2;
	stringFileSize.position.x=this->size.x/2;
	//锚点
	stringFileName.anchorPoint.x=0;
	stringFileSize.anchorPoint.x=1;
	//添加
	addSubObject(&stringFileName);
	addSubObject(&stringFileSize);
}

GameTable_Dir::GameTable_Dir():itemArray(nullptr){
	//设定数据源
	renderItemAmount=10;
	itemArray=new GameTable_DirItem[renderItemAmount];
}
GameTable_Dir::~GameTable_Dir(){delete []itemArray;}

void GameTable_DirItem::setSelected(bool b){
	bgColor=b ? &ColorRGBA::White : nullptr;
	stringFileName.color = b ? ColorRGBA::Black : ColorRGBA::White;
	stringFileSize.color = stringFileName.color;
}

bool GameTable_Dir::changeDir(const string &dirName,WhenErrorString whenError){
	bool b=directory.changeDir(dirName,whenError);
	if(b){
		directory.direntList.sortBy(DirentList::ByTypeAndName);
		renderItemStart=selectingItemIndex=0;
		updateRenderParameters();
	}
	return b;
}
const DirectoryEntry* GameTable_Dir::selectingDirectoryEntry()const{
	return directory.direntList.data(selectingItemIndex);
}

void GameTable_Dir::addItem(){
	addSubObject(&itemArray[subObjects.size()]);
}
void GameTable_Dir::removeItem(){
	removeSubObject(&itemArray[subObjects.size()-1]);
}
void GameTable_Dir::updateItemsData(){
	auto pos=renderItemStart;
	forEachSubObj<GameTable_DirItem>([&](GameTable_DirItem *dirItem){
		auto data=directory.direntList.data(pos);
		if(data){
			dirItem->stringFileName.setString(data->name());
			dirItem->stringFileSize.setString(data->strSize());
		}
		++pos;//下一个
	});
}
void GameTable_Dir::updateSelectCursor(){
	auto pos=renderItemStart;
	forEachSubObj<GameTable_DirItem>([&](GameTable_DirItem *dirItem){
		dirItem->setSelected(pos==selectingItemIndex);
		++pos;//下一个
	});
}
void GameTable_Dir::updateSize(){//定死尺寸
	auto &item=itemArray[0];
	size.setXY(item.size.x,item.size.y*renderItemAmount);
}

SizeType GameTable_Dir::rowAmount()const{return directory.direntList.size();}
uint GameTable_Dir::columnAmount()const{return 3;}
uint GameTable_Dir::columnWidth(uint col)const{
	switch(col){
		case FileName:return 400;
		case FileSize:return 120;
		default:return 0;
	}
}