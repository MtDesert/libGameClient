#ifndef CLIENT_H
#define CLIENT_H

#include"GameSettings.h"

#include"Transceiver.h"
#include"StringList.h"
#include"Protocol.h"

//客户端,用于向服务端发送请求,成员函数基本都会执行网络发送数据的动作
//正常情况下,Client对象在客户端中只需要一个,如果需要连接多个端口,应当考虑扩展Client中的socket个数
class GameClient:public Transceiver{
	StringList filenamesList;//文件名列表,用于批量下载
	string currentReqStr;//当前请求的字符串
public:
	GameClient();
	~GameClient();

	GameSettings *gameSettings;//游戏设置,从设置中读取网络设置

	//连接发送过程
	virtual bool sendData();//发送数据,自带重连机制
	void startUpgrade();//根据文件列表开始进行更新

	//向服务器发送请求
	void reqUpdateSOfiles(const string &gameName,const string &platform);//请求更新库文件
	void reqUpgradeSOfiles(const string &filename);//升级库文件
	//通用玩家请求
	void reqRegister(const string &gameName,const string &username,const string &password);//注册账户
	void reqLogin(const string &gameName,const string &username,const string &password);//登入
	void reqLogout();//登出
	//客户端事件
#define WHEN(name) \
	static void whenClient##name(Transceiver*);\
	void whenClient##name();
	TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN
	WhenErrorString whenClientErrorStr;

	//响应服务器发回的反馈
#define RESP(name) void resp##name(SocketDataBlock &data);
	CLIENT_REQUEST(RESP)
#undef RESP
#define RESP(name) function<void(SocketDataBlock&)> when##name##OK;
	PLAYER_REQUEST(RESP)
#undef RESP
};
#endif