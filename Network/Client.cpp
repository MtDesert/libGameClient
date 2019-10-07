#include"Client.h"
#include"Protocol.h"
#include"Game.h"

//静态存储区
static Client *client=nullptr;
static char sendBuffer[1024];
static SocketDataBlock clientDataBlock;

//socket事件
static void whenSocketError(Socket *socket){
	printf("socket error: %d\n",socket->errorNumber);
	if(client->whenError)client->whenError();
}
static void whenSocketConnected(Socket *socket){
	client->reqSelectGame(Game::currentGame()->gameName());
	if(client->whenConnected)client->whenConnected();
}
static void whenSocketSent(Socket *socket){
	printf("发送数据量%lu\n",socket->sentData.dataLength);
}
static void whenSocketReceived(Socket *socket){
	printf("接收数据量%lu\n",socket->recvData.dataLength);
}

//Client类
Client::Client():serverIPaddress(nullptr),serverPort(nullptr),whenError(nullptr),whenConnected(nullptr){
	//socket回调函数
	socket.whenSocketError=whenSocketError;
	socket.whenSocketConnected=whenSocketConnected;
	socket.whenSocketSent=whenSocketSent;
	socket.whenSocketReceived=whenSocketReceived;
	client=this;
	//缓冲
	clientDataBlock.set(sendBuffer,sizeof(sendBuffer));
}
void Client::sendData(void *dataPtr,size_t size){
	switch(socket.getConnectStatus()){
		case Socket::Unconnected:
			if(serverIPaddress && serverPort){
				toSendData.set(dataPtr,size);//保存要发的数据
				socket.connect(*serverIPaddress,*serverPort);//重连
			}
		break;
		case Socket::Connected:
			socket.send(dataPtr,size);
		break;
		default:;
	}
}

#define CLIENT_READY_SEND(request)\
	clientDataBlock.readyReadWrite();\
	clientDataBlock.add((uint8)request)
#define CLIENT_SEND sendData(clientDataBlock.dataPointer,clientDataBlock.rwSize);

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