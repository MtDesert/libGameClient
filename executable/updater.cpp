#include"Directory.h"
#include"ErrorNumber.h"
#include"GameClient.h"
#include"GameSettings.h"

#include<unistd.h>
#include<pthread.h>

#define WRITE_STR(str) fprintf(updateFile,"%s%c",(str),0);

//更新器,
int main(int argc,char* argv[]){
	Socket socket;
	//文件列表发送到服务器
	GameClient client;
	GameSettings settings;
	client.gameSettings=&settings;
	settings.serverIP="127.0.0.1";
	settings.serverPort=2048;
	client.setSocket(socket);
	client.reqUpdateSOfiles("AdvanceWars","Linux");
	//事件循环
	while(true){
		if(client.epollWait()<=0)pthread_yield();
	}
	return 0;
}