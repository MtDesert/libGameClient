#include"Client.h"
#include"Protocol.h"
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
	client->sendData();//发送还未发出的数据
	CLIENT_EVENT(Connected)
}
static void whenSocketSent(Socket *socket){
	printf("socket发送数据%lu\n",socket->sentData.dataLength);
	GET_CLIENT
	client->clearCache();//发送完毕时候要清除缓冲
	CLIENT_EVENT(Sent)
}
static void whenSocketReceived(Socket *socket){
	printf("socket接收数据%lu\n",socket->recvData.dataLength);
	GET_CLIENT
	CLIENT_EVENT(Received)
}

//Client类
Client::Client():serverIPaddress(nullptr),serverPort(0),
	whenError(nullptr),whenConnected(nullptr),whenSent(nullptr),whenReceived(nullptr){
	ErrorNumber::init();
	//socket回调函数
	socket.whenSocketError=whenSocketError;
	socket.whenSocketConnected=whenSocketConnected;
	socket.whenSocketSent=whenSocketSent;
	socket.whenSocketReceived=whenSocketReceived;
	socket.userData=this;
	//缓冲
	clientDataBlock.memoryAllocate(1024);
}
Client::~Client(){
	clientDataBlock.memoryFree();
}

void Client::sendData(){
	switch(socket.getConnectStatus()){
		case Socket::Unconnected://尚未连接,需要重连
			if(serverIPaddress){
				socket.connect(serverIPaddress,serverPort);
			}
		break;
		case Socket::Connected://已经连接,可发数据
			socket.send(clientDataBlock.dataPointer,clientDataBlock.rwSize);//发送数据
		break;
		default:;
	}
}
void Client::clearCache(){clientDataBlock.readyReadWrite();}
void Client::disconnect(){
	socket.close();
	socket.waitCloseFinish();
}

#define CLIENT_READY_SEND(request)\
	clientDataBlock.readyReadWrite();\
	clientDataBlock.add(string(#request))
#define CLIENT_SEND sendData();

void Client::reqUpdate(const DataBlock &fileList){
	CLIENT_READY_SEND(Update).add(fileList);
	CLIENT_SEND
}
void Client::reqSelectGame(const string &name){
	CLIENT_READY_SEND(enumSelectGame).add(name);
	CLIENT_SEND
}
void Client::reqRegister(const string &username, const string &password){
	CLIENT_READY_SEND(enumRegister).add(username).add(password);
	CLIENT_SEND
}
void Client::reqLogin(const string &username,const string &password){
	CLIENT_READY_SEND(enumLogin).add(username).add(password);
	CLIENT_SEND
}
void Client::reqLogout(){
	CLIENT_READY_SEND(enumLogout);
	CLIENT_SEND
}