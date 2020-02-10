#ifndef GAME_H
#define GAME_H

#include"GameObject.h"
#include"GameSettings.h"
#include"GameScene_FileList.h"
#include"GameScene_Logo.h"
#include"GameLayer_Conversation.h"
#include"GameTimer.h"
#include"GameScript.h"
#include"Client.h"

//场景跳转定义
#define GAME_GOTOSCENE_DEFINE(GameName,name)\
Scene_##name* Game_##GameName::gotoScene_##name(bool reset){\
	if(!scene##name){\
		scene##name=new Scene_##name();\
	}\
	gotoScene(*scene##name,reset);\
	return scene##name;\
}

/** Game是整个游戏运行的环境，游戏的主要数据都在本类中
该类主要是内存操作，与现有的游戏（或非游戏）图形环境等只做对接，比如glfw,glut,Qt,SDL,SFML等各种数不清的图形环境
可以接受控制设备的输入，但是控制设备是怎么工作的，这是图形库或者系统相关的图形界面程序要去考虑的事情，本类并不考虑，只提供成员函数来接受输入
*/

class Game:public GameObject{
public:
	Game();
	~Game();

	static Game* newGame();//创建游戏,请在子类实现
	static Game* currentGame();//当前游戏,子类实现时候注意返回值
	virtual string gameName()const;//游戏的名字
	virtual void reset();
	//输入输出变量
	static Point3D<int> resolution;//分辨率
	static Point2D<int> mousePos;//鼠标位置
	TimeSliceList timeSliceList;//时间片列表,用于控制物体运动

	//配置
	GameSettings *gameSettings;

	//翻译
	bool loadTranslationFile(const string &filename);//加载翻译文件
	const char* translate(const string &english)const;//翻译(英文原文),返回译文,翻译失败则返回原文

	//对话层(可无)
	GameLayer_Conversation* layerConversation;
#define GAME_USE_CONVERSATION(ClassName)\
	ClassName* useLayerConversation(){\
		if(!layerConversation){\
			layerConversation=new ClassName();\
		}\
		return dynamic_cast<ClassName*>(layerConversation);\
	}
	//剧情脚本(可无)
	GameScript* scenarioScript;
	//剧情脚本使用函数,给Game的子类使用,ClassName代表GameScript的子类
#define GAME_USE_SCRIPT(ClassName)\
	ClassName* useScenarioScript(){\
		if(!scenarioScript){\
			scenarioScript=new ClassName();\
			scenarioScript->whenError=whenError;\
			GameScript::gameScript=scenarioScript;\
		}\
		return dynamic_cast<ClassName*>(scenarioScript);\
	}
	virtual bool executeScript();//执行脚本命令

	//客户端
	static Client* currentClient();

	//提示框
	static void showDialogMessage(const string &content);
	static void hideDialogMessage();
	//场景管理
	void clearAllScenes();//清除所有场景
	GameScene* gotoScene(GameScene &scene,bool reset=false);//场景跳转
	GameScene_Logo* gotoScene_Logo();//游戏标志
	GameScene_FileList* gotoScene_FileList();//文件选择场景
	//错误处理
	static void whenError(const string &errStr);
	static void clearErrorMessages();

	//重写方法
	bool mouseMove(int x,int y);
	virtual void addTimeSlice(uint msec);
protected:
	static Game *game;//运行中的游戏
	Map<string,string> translationMap;//翻译表
};
#endif