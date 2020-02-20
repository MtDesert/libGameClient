#include"Game.h"
#include"GameDialog_Message.h"
#include"ErrorNumber.h"

#define ALL_COMMON_SCENE(MACRO)\
MACRO(Logo)\
MACRO(FileList)

#define SCENE_DECL(name) static GameScene_##name *scene##name=nullptr;//文件场景
ALL_COMMON_SCENE(SCENE_DECL)

Game::Game():gameSettings(nullptr),layerConversation(nullptr),scenarioScript(nullptr){}
Game::~Game(){
	//删除控件
	deleteSubObject(sceneLogo);
	deleteSubObject(sceneFileList);
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
void Game::reset(){
	ErrorNumber::init();//初始化错误字符串
	//加载字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.loadFile("fonts/ascii",whenError);
	cache.bitmapFontGb2312.symbolBlock.loadFile("fonts/lv0",whenError);
	cache.bitmapFontGb2312.lv1Chinese.loadFile("fonts/lv1",whenError);
	cache.bitmapFontGb2312.lv2Chinese.loadFile("fonts/lv2",whenError);
}

bool Game::loadTranslationFile(const string &filename){
	translationMap.clear();
	auto file=fopen(filename.data(),"r");
	if(!file){
		whenError(filename+": "+strerror(errno));
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
	fclose(file);
	return true;
}
const char* Game::translate(const string &english)const{
	auto value=translationMap.value(english);
	return value ? value->data() : english.data();
}
//脚本执行
bool Game::executeScript(){
	if(!scenarioScript)return false;
	//说话命令
	if(scenarioScript->strSay){//说话命令
		if(layerConversation)layerConversation->setDialogText(scenarioScript->strSay);
		scenarioScript->strSay=nullptr;
	}
	//设置形象
	if(scenarioScript->strBody){
		if(layerConversation)layerConversation->setBodyImage(gameSettings->bodyImagePath+"/"+scenarioScript->strBody+".png");
		scenarioScript->strBody=nullptr;
	}
	//执行完毕
	return true;
}

//客户端
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
//场景管理
static bool isScene(GameObject* const &obj){return obj && dynamic_cast<GameScene*>(obj);}
void Game::clearAllScenes(){subObjects.remove_if(isScene);}
GameScene* Game::gotoScene(GameScene &scene,bool reset){
	clearAllScenes();
	addSubObject(&scene,true);
	if(reset)scene.reset();
	return &scene;
}

#define SCENE_DEFINE(name)\
GameScene_##name* Game::gotoScene_##name(){\
	if(!scene##name){\
		scene##name=new GameScene_##name();\
	}\
	gotoScene(*scene##name);\
	return scene##name;\
}
ALL_COMMON_SCENE(SCENE_DEFINE)

//错误处理
static StringList allErrorStrings;//所有错误信息
static const char* errorString=nullptr;
void Game::whenError(const string &errStr){
	errorString=nullptr;
	allErrorStrings.push_back(errStr);
	//设置首个错误信息
	errorString=allErrorStrings.front().data();
	printf("error: %s\n",errStr.data());
}
void Game::clearErrorMessages(){allErrorStrings.clear();}

//重写函数
bool Game::mouseMove(int x,int y){
	mousePos.setXY(x,y);
	return GameObject::mouseMove(x,y);
}
void Game::addTimeSlice(uint msec){
	timeSliceList.addTimeSlice(msec);//计时器传递
	executeScript();
	//错误显示
	if(errorString){
		showDialogMessage(errorString);
		errorString=nullptr;
	}
}