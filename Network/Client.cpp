#include"Client.h"
#include"Directory.h"
#include"ErrorNumber.h"

#define GET_CLIENT auto client=reinterpret_cast<Client*>(socket->userData);
#define CLIENT_EVENT(Name) if(client->when##Name)client->when##Name(client);

//socket事件
static void whenSocketError(Socket *socket){
	printf("socket出错%d:\n",socket->errorNumber);
	GET_CLIENT
	CLIENT_EVENT(Error)
}
static void whenSocketConnected(Socket *socket){
	printf("socket已经连接\n");
	GET_CLIENT
	CLIENT_EVENT(Connected)
	client->sendData();//连接成功后,发送数据(如果有的话)
}
static void whenSocketDisconnected(Socket *socket){
	printf("socket断开连接\n");
	GET_CLIENT
	CLIENT_EVENT(Disconnected)
}

//Client类
Client::Client():fileToSend(NULL),fileToRecv(NULL),serverIPaddress(nullptr),serverPort(0),
	whenError(nullptr),whenConnected(nullptr),whenSent(nullptr),whenReceived(nullptr){
	ErrorNumber::init();
	//socket回调函数
	socket.whenSocketError=whenSocketError;
	socket.whenSocketConnected=whenSocketConnected;
	socket.whenSocketSent=whenSocketSent;
	socket.whenSocketReceived=whenSocketReceived;
	socket.whenSocketDisconnected=whenSocketDisconnected;
	socket.userData=this;
	//缓冲
	dataToSend.memoryAllocate(BUFSIZ);
}
Client::~Client(){
	dataToSend.memoryFree();
}

//Socket事件
void Client::whenSocketSent(Socket *socket){
	//printf("socket发送数据%lu\n",socket->sentData.dataLength);
	GET_CLIENT
	CLIENT_EVENT(Sent)
	//client->sendData();
}
void Client::whenSocketReceived(Socket *socket){
	printf("socket接收数据%lu\n",socket->recvData.dataLength);
	GET_CLIENT
	//对数据进行读取分析,并执行相关操作
	SocketDataBlock data;
	data.set(socket->recvData);
	data.readyReadWrite();
	//读取回复码
	string respCode;
	data.read(respCode);
	if(respCode=="ERR"){//服务端返回错误
		string errStr;
		data.read(errStr);
		printf("服务端报错:%s\n",errStr.data());
	}else if(respCode=="OK"){//服务器执行成功,读取剩余内容
		printf("开始执行命令%s\n",client->currentReqStr.data());
		//执行命令
#define CASE(command) if(strcmp(client->currentReqStr.data(),#command)==0){client->resp##command(data);}else
		CASE(UpdateSOfiles)
		{printf("未知命令%s\n",client->currentReqStr.data());}
	}else if(respCode=="DATA"){//大批量数据,一般是文件
#undef CASE
	}else{//服务器返回了未定义的代码
		printf("未知回复%s\n",respCode.data());
	}
	client->currentReqStr.clear();
}

void Client::sendData(){
	if(serverIPaddress){
		if(socket.send(dataToSend.dataPointer,dataToSend.rwSize)){//已经连接,发送数据应该没问题
		}else{//尚未连接,先进行连接
			socket.connect(serverIPaddress,serverPort);
		}
	}
}

static const char* upgradeFolderName="update";
void Client::startUpgrade(){
	if(filenamesList.size()<=0)return;//无文件不升级
	//创建更新目录
	auto dir=opendir(upgradeFolderName);
	if(dir)closedir(dir);//目录已经存在
	else{//目录不存在,创建之
		if(mkdir(upgradeFolderName,S_IRWXU)==0){}else{
			printf("无法创建更新目录%s\n",upgradeFolderName);
			return;
		}
	}
	//开始升级
	auto filename=filenamesList.front();
	fileToRecv=fopen((string(upgradeFolderName)+"/"+filename).data(),"wb");
	if(fileToRecv){
		reqUpgradeSOfiles(filename);//发送升级请求
	}else{
		printf("无法写文件%s\n",filename.data());
	}
}
int Client::epollWait(){return socket.epollWait();}

//请求模块
#define CLIENT_READY_SEND(request)\
	currentReqStr=#request;\
	dataToSend.readyReadWrite();\
	dataToSend.add(#request)
#define CLIENT_SEND sendData();

void Client::reqUpdateSOfiles(const string &gameName,const string &platform){
	filenamesList.clear();//要清空文件列表
	CLIENT_READY_SEND(UpdateSOfiles).add(gameName).add(platform);
	CLIENT_SEND
}
void Client::reqUpgradeSOfiles(const string &filename){
	CLIENT_READY_SEND(UpgradeSOfiles).add(filename);
	CLIENT_SEND
}
void Client::reqUpdate(const DataBlock &fileList){
	CLIENT_READY_SEND(Update).add(fileList);
	CLIENT_SEND
}
void Client::reqSelectGame(const string &name){
	CLIENT_READY_SEND(SelectGame).add(name);
	CLIENT_SEND
}
void Client::reqRegister(const string &username, const string &password){
	CLIENT_READY_SEND(Register).add(username).add(password);
	CLIENT_SEND
}
void Client::reqLogin(const string &username,const string &password){
	CLIENT_READY_SEND(Login).add(username).add(password);
	CLIENT_SEND
}
void Client::reqLogout(){
	CLIENT_READY_SEND(Logout);
	CLIENT_SEND
}

//响应模块
void Client::respUpdateSOfiles(SocketDataBlock &data){
	string filename;
	DirectoryEntry localEntry,remoteEntry;
	while(data.rwSize<data.dataLength){
		data.read(filename).read(remoteEntry.structStat.st_mtime);//读取服务器的文件名和时间
		if(::stat(filename.data(),&localEntry.structStat)==0){//读取本地文件的时间
			if(localEntry.structStat.st_mtime < remoteEntry.structStat.st_mtime){//需要更新
				printf("old %s new %s %s\n",localEntry.strModifyTime().data(),remoteEntry.strModifyTime().data(),filename.data());
				filenamesList.push_back(filename);
			}
		}else{//文件不存在,也要更新
			filenamesList.push_back(filename);
		}
	}
	//开始更新文件
	startUpgrade();
}
void Client::respUpgradeSOfiles(SocketDataBlock &data){
	if(!fileToRecv){//更新文件
	}
}