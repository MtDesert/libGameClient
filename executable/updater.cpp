#include"Directory.h"
#include"ErrorNumber.h"
#include"Client.h"

#include<unistd.h>
const char* fileList="update/fileList";

static void whenError(const string &str){
	printf("error: %s\n",str.data());
}
static void whenError(Client *client){
	printf("客户端:出错\n");
}
static void whenConnetced(Client *client){
	printf("客户端:已经连接\n");
}

int main(int argc,char* argv[]){
	//建立更新文件夹
	ErrorNumber::init();
	errno=0;
	if(mkdir("update",S_IRWXU) && errno!=EEXIST){
		printf("error %d: %s\n",errno,ErrorNumber::getErrorString(errno));
		return -1;
	}
	//准备更新内容
	auto updateFile=fopen(fileList,"w");
	if(!updateFile){
		printf("error %d: %s\n",errno,ErrorNumber::getErrorString(errno));
		return -1;
	}
	//扫描本地动态库
	Directory dir;
	if(dir.changeDir(".",whenError)){//切换目录,得到列表
		dir.direntList.forEach([&](DirectoryEntry &entry){
			auto nm=entry.name();
			if(nm.find(".so")==nm.length()-3 && entry.isRegularFile()){//只更新.so文件
				printf("%s %s\n",entry.strModifyTime().data(),entry.d_name);
				fprintf(updateFile,"%s%c",entry.d_name,0);
				fwrite(&entry.structStat.st_mtime,sizeof(entry.structStat.st_mtime),1,updateFile);
			}
		});
	}
	fclose(updateFile);
	//文件列表发送到服务器
	DataBlock block;
	block.loadFile(fileList);
	Client client;
	client.serverIPaddress="127.0.0.1";
	client.serverPort=2048;
	client.whenError=whenError;
	client.whenConnected=whenConnetced;
	client.reqUpdate(block);
	//等待
	sleep(1);
	client.disconnect();
	return 0;
}