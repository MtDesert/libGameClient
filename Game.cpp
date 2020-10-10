#include"Game.h"
#include"GameInputBox.h"
#include"Dialog_Message.h"
#include"ErrorNumber.h"

//宏定义开始
#define ALL_COMMON_SCENE(MACRO)\
MACRO(Logo)\
MACRO(FileList)\
MACRO(Background)

#define ALL_COMMON_DIALOG(MACRO)\
MACRO(Login)\
MACRO(Message)
//宏定义结束

ALL_COMMON_SCENE(GAME_SCENE_DECLARE)
ALL_COMMON_DIALOG(GAME_DIALOG_DECLARE)

Game::Game():gameSettings(nullptr),gameClient(nullptr),layerConversation(nullptr),scenarioScript(nullptr){}
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
	//读取配置
	if(gameSettings){
		gameSettings->loadFile("settings.lua");//读取配置
		resolution.setP(gameSettings->resolution);//设定分辨率
		loadTranslationFile(gameSettings->language+".csv");//读取翻译文件
	}
	//设置网络模块(部分游戏可能没有)
	if(gameClient){
		gameClient->gameSettings=this->gameSettings;
		gameClient->whenClientErrorStr=whenError;
	}
	//加载字体
	FontTextureCache &cache(GameString::fontTextureCache);
	cache.bitmapFontAscii.charBlock.loadFile("fonts/ascii",whenError);
	cache.bitmapFontGb2312.symbolBlock.loadFile("fonts/lv0",whenError);
	cache.bitmapFontGb2312.lv1Chinese.loadFile("fonts/lv1",whenError);
	cache.bitmapFontGb2312.lv2Chinese.loadFile("fonts/lv2",whenError);
}
void Game::restart(){}

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
	if(scenarioScript->backgroundName){//显示背景
		auto scene=gotoScene_Background(true);
		scene->setBackground(scenarioScript->backgroundName);
		scenarioScript->backgroundName=nullptr;
	}
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

//场景管理
static bool isGameScene(GameObject *obj){return obj && dynamic_cast<GameScene*>(obj);}
void Game::clearAllScenes(){subObjects.remove_if(isGameScene);}
GameScene* Game::findFirstScene()const{
	auto pObj=subObjects.data(isGameScene);
	return pObj ? dynamic_cast<GameScene*>(*pObj) : nullptr;
}
GameScene* Game::gotoScene(GameScene &scene,bool reset){
	if(reset)scene.reset();
	clearAllScenes();
	addSubObject(&scene,true);
	return &scene;
}

GAME_GOTOSCENE_DEFINE(Game,Logo)
GAME_GOTOSCENE_DEFINE(Game,FileList)
GAME_GOTOSCENE_DEFINE(Game,Background)
GAME_SHOWDIALOG_DEFINE(Game,Login)
GAME_SHOWDIALOG_DEFINE(Game,Message)

//对话框
GameDialog* Game::showDialog(GameDialog &dialog){
	addSubObject(&dialog);
	return &dialog;
}
Dialog_Message* Game::dialogOK(const string &text){
	auto dialog=Game::currentGame()->showDialog_Message();
	dialog->buttonsConfirmCancel.showButtonCancel(false);
	dialog->setText(text);
	return dialog;
}
Dialog_Message* Game::dialogConfirm(const string &text,GameButton::ClickCallback confirmCallback){
	auto dialog=Game::currentGame()->showDialog_Message();
	dialog->buttonsConfirmCancel.showButtonCancel(true);
	dialog->setText(text);
	dialog->setConfirmCallback(confirmCallback);
	return dialog;
}
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
void Game::addTimeSlice(uint ms){
	timeSliceList.addTimeSlice(ms);//计时器传递
	countDownList.countingDown(ms);
	executeScript();
	//处理数据输入
	GameInputBox::updateInput();
	//错误显示
	if(errorString){
		auto dialog=dialogOK(errorString);
		dialog->setConfirmCallback([&](){
			clearErrorMessages();
		});
		errorString=nullptr;//防止频繁进入
	}
}