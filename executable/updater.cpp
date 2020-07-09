#include"Directory.h"
#include"ErrorNumber.h"
#include"Client.h"

#include<unistd.h>
#include<pthread.h>

static void whenError(Client *client){
	printf("客户端:出错\n");
}
static void whenConnetced(Client *client){
	printf("客户端:已经连接\n");
}
static void whenSent(Client *client){
	printf("客户端:发送数据\n");
}
static void whenReceived(Client *client){
	printf("客户端:接收数据\n");
}

#define WRITE_STR(str) fprintf(updateFile,"%s%c",(str),0);

//更新器,
int main(int argc,char* argv[]){
	//文件列表发送到服务器
	Client client;
	client.serverIPaddress="127.0.0.1";
	client.serverPort=2048;
	client.whenError=whenError;
	client.whenConnected=whenConnetced;
	client.whenSent=whenSent;
	client.whenReceived=whenReceived;
	client.reqUpdateSOfiles("AdvanceWars","Linux");
	//事件循环
	while(true){
		if(client.epollWait()<=0)pthread_yield();
	}
	return 0;
}