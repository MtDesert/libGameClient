#include"Game.h"
#include"GameDialog_Message.h"

Game::Game():sceneFileList(nullptr){
	//加载字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.loadFile("fonts/ascii");
	cache.bitmapFontGb2312.symbolBlock.loadFile("fonts/lv0");
	cache.bitmapFontGb2312.lv1Chinese.loadFile("fonts/lv1");
	cache.bitmapFontGb2312.lv2Chinese.loadFile("fonts/lv2");
}
Game::~Game(){
	//删除控件
	deleteScene_FileList();
	//删除字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.memoryFree();
	cache.bitmapFontGb2312.symbolBlock.memoryFree();
	cache.bitmapFontGb2312.lv1Chinese.memoryFree();
	cache.bitmapFontGb2312.lv2Chinese.memoryFree();
}

//Game* Game::newGame(){return new Game();}

#define STATIC(name)\
decltype(name) name

STATIC(Game::game)=nullptr;
STATIC(Game::resolution);
STATIC(Game::mousePos);

Game* Game::currentGame(){return Game::game;}
string Game::gameName()const{return"";}

bool Game::loadTranslationFile(const string &filename){
	translationMap.clear();
	auto file=fopen(filename.data(),"r");
	if(!file){
		switch(errno){
		}
		return false;
	}
	//开始读取映射
	int bufferSize=256;
	char buffer[bufferSize];
	char *start,*finish;
	while(fgets(buffer,bufferSize,file)){
		//',' -> '\0'
		start=strstr(buffer,",");
		if(!start)continue;
		*start='\0';//',' -> '\0'
		++start;
		//'\n' -> '\0'
		finish=strstr(start,"\n");
		if(finish)*finish='\0';
		//开始插入映射
		translationMap.insert(buffer,start);
	}
	return true;
}
const char* Game::translate(const string &english)const{
	auto value=translationMap.value(english);
	return value ? value->data() : english.data();
}

GameScene_FileList *Game::showScene_FileList(){
	if(!sceneFileList){
		sceneFileList=new GameScene_FileList();
		gotoScene(sceneFileList);
	}
	return sceneFileList;
}
void Game::deleteScene_FileList(){
	if(sceneFileList){
		removeSubObject(sceneFileList);
		delete sceneFileList;
	}
	sceneFileList=nullptr;
}

static Client *client=nullptr;
Client* Game::currentClient(){
	if(!client){
		client=new Client();
	}
	return client;
}

//消息框
static GameDialog_Message *messageDialog=nullptr;
void Game::showDialogMessage(const string &content){
	if(!messageDialog){
		messageDialog=new GameDialog_Message();
		currentGame()->addSubObject(messageDialog);
	}
	messageDialog->setText(content);
}
void Game::hideDialogMessage(){
	currentGame()->removeSubObject(messageDialog);
	delete messageDialog;
	messageDialog=nullptr;
}

//重写函数
void Game::mouseMove(int x,int y){
	mousePos.x=x;
	mousePos.y=y;
	GameObject::mouseMove(x,y);
}
void Game::addTimeSlice(uint usec){
	GameObject::addTimeSlice(usec);
	timeSliceList.addTimeSlice(usec);
}

static bool isScene(GameObject* const &obj){return obj && dynamic_cast<GameScene*>(obj);}
void Game::clearAllScenes(){subObjects.remove_if(isScene);}
void Game::gotoScene(GameScene *scene){
	clearAllScenes();
	addSubObject(scene,true);
}