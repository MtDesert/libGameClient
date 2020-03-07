#ifndef GAME_H
#define GAME_H

#include"GameObject.h"
#include"GameSettings.h"
#include"Scene_FileList.h"
#include"Scene_Logo.h"
#include"GameLayer_Conversation.h"
#include"Dialog_Login.h"
#include"Dialog_Message.h"
#include"GameTimer.h"
#include"GameScript.h"
#include"Client.h"

//以下宏定义建议放在Game子类的.cpp文件
//场景声明,对话框声明
#define GAME_SCENE_DECLARE(name) static Scene_##name *scene##name=nullptr;
#define GAME_DIALOG_DECLARE(name) static Dialog_##name *dialog##name=nullptr;
//场景跳转定义
#define GAME_GOTOSCENE_DEFINE(GameClass,name)\
Scene_##name* GameClass::gotoScene_##name(bool reset){\
	if(!scene##name){\
		scene##name=new Scene_##name();\
	}\
	gotoScene(*scene##name,reset);\
	return scene##name;\
}
//对话框显示定义
#define GAME_SHOWDIALOG_DEFINE(GameClass,name)\
Dialog_##name* GameClass::showDialog_##name(){\
	if(!dialog##name){\
		dialog##name=new Dialog_##name();\
	}\
	showDialog(*dialog##name);\
	return dialog##name;\
}
//场景删除,对话框删除
#define GAME_DELETE_SCENE(name) deleteSubObject(scene##name);
#define GAME_DELETE_DIALOG(name) deleteSubObject(dialog##name);

/** Game是整个游戏运行的环境，游戏的主要数据都在本类中
该类主要是内存操作，与现有的游戏（或非游戏）图形环境等只做对接，比如glfw,glut,Qt,SDL,SFML等各种数不清的图形环境
可以接受控制设备的输入，但是控制设备是怎么工作的，这是图形库或者系统相关的图形界面程序要去考虑的事情，本类并不考虑，只提供成员函数来接受输入
该类主要负责:配置读写,翻译,资源文件调度,场景,对话框等资源
*/

class Game:public GameObject{
public:
	Game();
	virtual ~Game();

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

	//场景管理
	void clearAllScenes();//清除所有场景
	GameScene* findFirstScene()const;//寻找首个场景
	GameScene* gotoScene(GameScene &scene,bool reset=false);//场景跳转,返回scene的指针
	Scene_Logo* gotoScene_Logo(bool reset=false);//场景:游戏标志
	Scene_FileList* gotoScene_FileList(bool reset=false);//场景:文件选择
	//对话框
	GameDialog* showDialog(GameDialog &dialog);
	Dialog_Login* showDialog_Login();
	Dialog_Message* showDialog_Message();
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