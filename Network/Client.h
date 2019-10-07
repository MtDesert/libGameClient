#ifndef CLIENT_H
#define CLIENT_H

#include"Socket.h"

//客户端,用于向服务端发送请求,成员函数基本都会执行网络发送数据的动作
//正常情况下,Client对象在客户端中只需要一个,如果需要连接多个端口,应当考虑扩展Client中的socket个数
class Client{
	Socket socket;//用于向服务端收发数据
	DataBlock toSendData;//要发送的数据,在没有连接到服务端的时候会缓存在此
public:
	Client();
	//连接目标,在需要重新连接时候可用
	const string *serverIPaddress;
	const int *serverPort;

	//客户端回调函数
	void (*whenError)();//错误回调
	void (*whenConnected)();
	//连接发送过程
	void sendData(void *dataPtr,size_t size);
	//请求
	void reqSelectGame(const string &name);//选择游戏(名字)
	void reqRegister(const string &username,const string &password);//注册账户
	void reqLogin(const string &username,const string &password);//登陆
	void reqLogout();
	//响应
};
#endif