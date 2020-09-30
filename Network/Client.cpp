#include"Client.h"
#include"Directory.h"
#include"ErrorNumber.h"

static Socket clientSocket;//这是用于Client的socket,一般情况下只用一个,除非要连接多个服务
Client::Client():serverIPaddress(nullptr),serverPort(0),whenClientErrorStr(nullptr)
#define CALLBACK(name) ,when##name##OK(NULL)
	PLAYER_REQUEST(CALLBACK)
#undef CALLBACK
{
	setSocket(::clientSocket);
#define WHEN(name) whenTransceiver##name=whenClient##name;
	TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN
}
Client::~Client(){}

static const string upgradeFolderName="update";
bool Client::sendData(){
	bool ret=socket->isConnected();
	if(ret){
		printf("客户端传输数据\n");
		Transceiver::sendData();
	}else{//未连接,开始进行连接
		printf("客户端重连\n");
		if(serverIPaddress){
			socket->connect(serverIPaddress,serverPort);
		}
	}
	return ret;
}
void Client::startUpgrade(){
	if(filenamesList.size()<=0)return;//无文件不升级
	//创建更新目录
	if(!Directory::exist(upgradeFolderName) && !Directory::makeDirectory(upgradeFolderName)){//目录不存在,创建之
		printf("无法创建更新目录%s\n",upgradeFolderName.data());
		return;
	}
	//开始发送更新请求
	reqUpgradeSOfiles(filenamesList.front());
}

//请求模块
#define CLIENT_READY_SEND(request)\
	currentReqStr=#request;\
	readySend(#request)
#define CLIENT_SEND sendData();

void Client::reqUpdateSOfiles(const string &gameName,const string &platform){
	printf("请求:更新%s %s\n",gameName.data(),platform.data());
	filenamesList.clear();//要清空文件列表
	CLIENT_READY_SEND(UpdateSOfiles).write(gameName).write(platform);
	CLIENT_SEND
}
void Client::reqUpgradeSOfiles(const string &filename){
	printf("请求:升级文件%s\n",filename.data());
	CLIENT_READY_SEND(UpgradeSOfiles).write(filename);
	CLIENT_SEND
}
void Client::reqRegister(const string &gameName,const string &username,const string &password){
	CLIENT_READY_SEND(Register).write(gameName).write(username).write(password);
	CLIENT_SEND
}
void Client::reqLogin(const string &gameName, const string &username, const string &password){
	CLIENT_READY_SEND(Login).write(gameName).write(username).write(password);
	CLIENT_SEND
}
void Client::reqLogout(){
	CLIENT_READY_SEND(Logout);
	CLIENT_SEND
}
//事件模块
#define WHEN(name) \
void Client::whenClient##name(Transceiver *transceiver){\
	dynamic_cast<Client*>(transceiver)->whenClient##name();\
}
TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN

//响应模块
void Client::whenClientReceived(){
	//收到了完整指令
	string respCode;
	readBuffer.rwSize=writeBuffer.rwSize=0;
	readBuffer.read(packetLength).read(respCode);
	if(respCode=="OK"){//响应成功
		//处理系统请求,默认为Client的函数
#define CASE(name) if(currentReqStr==#name){resp##name(readBuffer);}else
		CLIENT_REQUEST(CASE)
#undef CASE
		//处理请求用户请求,调用各种游戏自定义的回调函数
#define CASE(name) if(currentReqStr==#name){if(when##name##OK)when##name##OK(readBuffer);}else
		PLAYER_REQUEST(CASE)
#undef CASE
		{}
	}else if(respCode=="FILE"){//数据,有可能是文件数据
		receiveFileData();
	}else if(respCode=="CMD"){//服务端推送
	}else if(respCode=="ERR"){//有错误
		readBuffer.read(respCode);
		if(whenClientErrorStr)whenClientErrorStr(respCode);
	}else{
	}
}
void Client::whenClientReceivedFile(){
	filenamesList.pop_front();//接收完毕,从名单中移除
	startUpgrade();//继续更新
}
void Client::whenClientError(){
	if(whenClientErrorStr)whenClientErrorStr(ErrorNumber::getErrorString(socket->errorNumber));//直接把socket的错误报上去
}

//响应模块
void Client::respUpdateSOfiles(SocketDataBlock &data){
	string filename;
	DirectoryEntry localEntry,remoteEntry;
	auto totalLen=defaultHeaderSize+packetLength;
	while(data.rwSize < totalLen){
		data.read(filename).read(remoteEntry.structStat.st_mtime);//读取服务器的文件名和时间
		//printf("old %s new %s %s\n",localEntry.strModifyTime().data(),remoteEntry.strModifyTime().data(),filename.data());
		if(::stat(filename.data(),&localEntry.structStat)==0){//读取本地文件的时间
			if(localEntry.structStat.st_mtime < remoteEntry.structStat.st_mtime){//需要更新
				filenamesList.push_back(filename);
			}
		}else{//文件不存在,也要更新
			filenamesList.push_back(filename);
		}
		filenamesList.push_back(filename);
	}
	//开始更新文件
	startUpgrade();
}
void Client::respUpgradeSOfiles(SocketDataBlock &data){
	data.read(recvFileSize);//获取文件接收大小
	receiveFile(upgradeFolderName+"/"+filenamesList.front(),recvFileSize);
	printf("准备更新文件%s大小%lu\n",filenamesList.front().data(),recvFileSize);
}