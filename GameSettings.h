#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include"LuaState.h"

//游戏设置,游戏可以有通用和专用的设置,以供玩家自行修改参数
//本类提供通用配置内容,如果是特定游戏的特殊设置请在子类实现
class GameSettings{
public:
	GameSettings();
	~GameSettings();

	//尺寸相关
	Point2D<int> windowSize;//窗口大小,(在带有窗体操作系统的情况下可以用)
	Point2D<int> resolution;//分辨率,决定绘制区域的清晰程度
	//通用配置内容
	string language;//游戏的语言,用于在加载时候读取对应的翻译文件
	string headImagePath;//头像路径,特定的游戏会用到
	string bodyImagePath;//全身像路径,特定的游戏会用到
	//网络部分
	string serverIP;
	uint16 serverPort;

	bool loadFile(const string &filename);
protected:
	virtual void readCustom(LuaState &state);//读取自定义设置,
};
#endif