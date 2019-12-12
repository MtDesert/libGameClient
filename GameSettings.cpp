#include"GameSettings.h"
#include"Game.h"

GameSettings::GameSettings(){}
GameSettings::~GameSettings(){}

#define READ_STR(name) luaState.getGlobalString(#name,name);

bool GameSettings::loadFile(const string &filename){
//尝试打开配置文件,出错了就直接返回错误信息
	LuaState luaState;
	luaState.whenError=Game::whenError;
	if(!luaState.doFile(filename))return false;
	//读取通用配置
	READ_STR(language)
	READ_STR(bodyImagePath)
	readCustom(luaState);
	//结束
	luaState.clearStack();
	return true;
}
void GameSettings::readCustom(LuaState &state){}