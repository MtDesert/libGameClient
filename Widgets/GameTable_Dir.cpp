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

GameTable_Dir::GameTable_Dir(){setRenderItemAmount(10);}
GameTable_Dir::~GameTable_Dir(){}

void GameTable_DirItem::updateData(SizeType pos){
	auto dirTable=dynamic_cast<GameTable_Dir*>(parentObject);
	if(dirTable){
		auto entry=dirTable->directory.direntList.data(pos);
		if(entry){
			stringFileName.setString(entry->name());
			stringFileSize.setString(entry->strSize());
		}
	}
}
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
		updateRenderParameters(true);
	}
	return b;
}
const DirectoryEntry* GameTable_Dir::selectingDirectoryEntry()const{
	return directory.direntList.data(selectingItemIndex);
}

void GameTable_Dir::updateSize(){//定死尺寸
	auto &item=itemArray[0];
	size.setXY(item.size.x,item.size.y*renderItemAmount);
}

SizeType GameTable_Dir::rowAmount()const{return directory.direntList.size();}
SizeType GameTable_Dir::columnAmount()const{return 2;}
SizeType GameTable_Dir::columnWidth(uint col)const{
	switch(col){
		case FileName:return 400;
		case FileSize:return 120;
		default:return 0;
	}
}