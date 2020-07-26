#include"Client.h"
#include"Directory.h"
#include"ErrorNumber.h"

//Client类
Client::Client():serverIPaddress(nullptr),serverPort(0){
#define WHEN(name) whenTransceiver##name=whenClient##name;
	TRANSCEIVER_ALL_EVENTS(WHEN)
#undef WHEN
}
Client::~Client(){}

static const string upgradeFolderName="update";
bool Client::sendData(){
	if(socket->isConnected){
		Transceiver::sendData();
	}else{//未连接,开始进行连接
		if(serverIPaddress){
			socket->connect(serverIPaddress,serverPort);
		}
	}
	return socket->isConnected;
}
void Client::startUpgrade(){
	if(filenamesList.size()<=0)return;//无文件不升级
	//创建更新目录
	auto dir=opendir(upgradeFolderName.data());
	if(dir)closedir(dir);//目录已经存在
	else{//目录不存在,创建之
		if(mkdir(upgradeFolderName.data(),S_IRWXU)==0){}else{
			printf("无法创建更新目录%s\n",upgradeFolderName.data());
			return;
		}
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
void Client::reqUpdate(const DataBlock &fileList){
	CLIENT_READY_SEND(Update).write(fileList);
	CLIENT_SEND
}
void Client::reqSelectGame(const string &name){
	CLIENT_READY_SEND(SelectGame).write(name);
	CLIENT_SEND
}
void Client::reqRegister(const string &username, const string &password){
	CLIENT_READY_SEND(Register).write(username).write(password);
	CLIENT_SEND
}
void Client::reqLogin(const string &username,const string &password){
	CLIENT_READY_SEND(Login).write(username).write(password);
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
#define CASE(name) if(currentReqStr==#name){resp##name(readBuffer);}else
		CASE(UpdateSOfiles)
		CASE(UpgradeSOfiles)
		{}
#undef CASE
	}else if(respCode=="FILE"){//数据,有可能是文件数据
		receiveFileData();
	}else if(respCode=="CMD"){//服务端推送
	}else if(respCode=="ERR"){//有错误
	}else{
	}
}
void Client::whenClientReceivedFile(){
	filenamesList.pop_front();//接收完毕,从名单中移除
	startUpgrade();//继续更新
}

//响应模块
void Client::respUpdateSOfiles(SocketDataBlock &data){
	string filename;
	DirectoryEntry localEntry,remoteEntry;
	auto totalLen=defaultHeaderSize+packetLength;
	while(data.rwSize < totalLen){
		data.read(filename).read(remoteEntry.structStat.st_mtime);//读取服务器的文件名和时间
		printf("old %s new %s %s\n",localEntry.strModifyTime().data(),remoteEntry.strModifyTime().data(),filename.data());
		/*if(::stat(filename.data(),&localEntry.structStat)==0){//读取本地文件的时间
			if(localEntry.structStat.st_mtime < remoteEntry.structStat.st_mtime){//需要更新
				filenamesList.push_back(filename);
			}
		}else{//文件不存在,也要更新
			filenamesList.push_back(filename);
		}*/
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