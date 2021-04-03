#include"GameClient.h"
#include"Directory.h"
#include"ErrorNumber.h"
#include"PrintF.h"

static Socket clientSocket;//这是用于Client的socket,一般情况下只用一个,除非要连接多个服务
GameClient::GameClient():whenClientErrorStr(nullptr)
#define CALL_BACK(name) ,when##name##OK(NULL)
	PLAYER_REQUEST(CALL_BACK)
#undef CALL_BACK
{
	setSocket(::clientSocket);
#define WHEN(name) whenTransceiver##name=whenClient##name;
	TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN
}
GameClient::~GameClient(){}

static const string upgradeFolderName="update";
bool GameClient::sendData(){
	bool ret=socket->isConnected();
	if(ret){
		Transceiver::sendData();
	}else{//未连接,开始进行连接
		PRINT_WARN("客户端重连\n");
		if(gameSettings){
			socket->connect(gameSettings->serverIP,gameSettings->serverPort);
		}
	}
	return ret;
}
void GameClient::startUpgrade(){
	if(filenamesList.size()<=0)return;//无文件不升级
	//创建更新目录
	if(!Directory::exist(upgradeFolderName) && !Directory::makeDirectory(upgradeFolderName)){//目录不存在,创建之
		PRINT_ERROR("无法创建更新目录%s\n",upgradeFolderName.data());
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

void GameClient::reqUpdateSOfiles(const string &gameName,const string &platform){
	PRINT_INFO("请求:更新%s %s\n",gameName.data(),platform.data());
	filenamesList.clear();//要清空文件列表
	CLIENT_READY_SEND(UpdateSOfiles).write(gameName).write(platform);
	CLIENT_SEND
}
void GameClient::reqUpgradeSOfiles(const string &filename){
	PRINT_INFO("请求:升级文件%s\n",filename.data());
	CLIENT_READY_SEND(UpgradeSOfiles).write(filename);
	CLIENT_SEND
}
void GameClient::reqRegister(const string &gameName,const string &username,const string &password){
	CLIENT_READY_SEND(Register).write(gameName).write(username).write(password);
	CLIENT_SEND
}
void GameClient::reqLogin(const string &gameName, const string &username, const string &password){
	CLIENT_READY_SEND(Login).write(gameName).write(username).write(password);
	CLIENT_SEND
}
void GameClient::reqLogout(){
	CLIENT_READY_SEND(Logout);
	CLIENT_SEND
}
//事件模块
#define WHEN(name) \
void GameClient::whenClient##name(Transceiver *transceiver){\
	dynamic_cast<GameClient*>(transceiver)->whenClient##name();\
}
TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN

//响应模块
void GameClient::whenClientReceived(){
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
void GameClient::whenClientReceivedFile(){
	filenamesList.pop_front();//接收完毕,从名单中移除
	startUpgrade();//继续更新
}
void GameClient::whenClientError(){
	if(whenClientErrorStr)whenClientErrorStr(ErrorNumber::getErrorString(socket->errorNumber));//直接把socket的错误报上去
}

//响应模块
void GameClient::respUpdateSOfiles(SocketDataBlock &data){
	string filename;
	DirectoryEntry localEntry,remoteEntry;
	auto totalLen=defaultHeaderSize+packetLength;
	while(data.rwSize < totalLen){
		data.read(filename).read(remoteEntry.structStat.st_mtime);//读取服务器的文件名和时间
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
void GameClient::respUpgradeSOfiles(SocketDataBlock &data){
	data.read(recvFileSize);//获取文件接收大小
	receiveFile(upgradeFolderName+"/"+filenamesList.front(),recvFileSize);
}