#include"GameScript.h"
#include"Game.h"
#ifdef __MINGW32__
#include<windows.h>
#endif

GameScript* GameScript::gameScript=nullptr;

//构造析构函数
GameScript::GameScript():whenError(nullptr),luaScript(nullptr),isScriptWaiting(false){}
GameScript::~GameScript(){if(luaScript)delete luaScript;}

//脚本函数专用宏
#define SCRIPT GameScript::gameScript
#define GET_STR(idx) (lua_type(state,(idx))==LUA_TSTRING ? lua_tostring(state,idx) : nullptr)
#define TO_STR(pStr,idx) auto pStr = GET_STR(idx);
#define SCRIPT_WAIT(time) SCRIPT->scriptWait(time);
#define SCRIPT_WHEN_ERROR(str) if(SCRIPT->whenError)SCRIPT->whenError(str);

//脚本的加载或执行
#define DO_OR_LOAD_SCRIPT(doOrLoad)\
int GameScript::doOrLoad##Script(lua_State *state){\
	const char* filename=GET_STR(1);\
	if(filename){\
		if(!SCRIPT->luaScript->doOrLoad##File(filename))return 1;\
	}else{\
		SCRIPT_WHEN_ERROR(string("Parameter of "#doOrLoad"Script not string!"));\
	}\
	return 0;\
}
DO_OR_LOAD_SCRIPT(do)
DO_OR_LOAD_SCRIPT(load)

int GameScript::fadeInBackground(lua_State *state){
	return lua_gettop(state);
}
int GameScript::say(lua_State *state){
	//TO_STR(strSay,1)//说话内容
	SCRIPT_WAIT()
	return 0;
}
int GameScript::nameSay(lua_State *state){
	//TO_STR(strName,1)
	//TO_STR(strSay,2)
	SCRIPT_WAIT()
	return 0;
}
int GameScript::headSay(lua_State *state){
	return bodySay(state);
}
int GameScript::bodySay(lua_State *state){
	//TO_STR(strBody,1)
	TO_STR(strSay,2)
	auto game=Game::currentGame();
	game->layerConversation->setDialogText(strSay);
	SCRIPT_WAIT()
	return 0;
}

//脚本初始化
void GameScript::scriptInit(){
	//注册函数
#define CASE(name) luaScript->registerFunction(#name,name);
	GAMESCRIPT_ALL_FUNCTIONS(CASE)
#undef CASE
}
bool GameScript::loadSenarioScript(const string &filename){
	//初始化脚本环境
	if(!luaScript){
		luaScript=new LuaState();
		luaScript->whenError=whenError;
		scriptInit();//(初始化,子类重写)
	}
	//加载脚本文件
	return luaScript->loadFile(filename);
}
bool GameScript::executeSenarioScript(){
	if(isScriptWaiting){//脚本等待状态,不需要往下执行
		return false;
	}else{//脚本可以继续执行,则唤醒lua线程
		auto ret=luaScript->resume();
		if(!ret)isScriptWaiting=true;//脚本执行错误,防止继续resume
		return ret;
	}
}
void GameScript::scriptWait(uint msec){
	isScriptWaiting=true;
	luaScript->yield();//脚本暂停执行
}
void GameScript::scriptContinue(){isScriptWaiting=false;}