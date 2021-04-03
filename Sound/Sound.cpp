#include"Sound.h"
#include"Charset.h"
#include"PrintF.h"

#ifdef __MINGW32__
#include<windows.h>
#include<mmsystem.h>

static void printError(const string &name,MCIERROR err){
	char info[256];	
	mciGetErrorString(err,info,256);
	//mciGetErrorString返回的是gbk,如果终端的字符集是utf8,则需要做如下转换
	Charset chset;
	chset.srcCharset=Charset::GBK;
	chset.destCharset=Charset::UTF8;
	auto block=chset.newString(info);
	PRINT_CYAN("%s:%s",name.data(),block.dataPointer)
}

bool Sound::playWav(const string &name){
	return sndPlaySound(("musics/"+name+".wav").data(),SND_FILENAME|SND_ASYNC|SND_LOOP);
}
bool Sound::playMp3(const string &name){
	auto ret=mciSendString(("play musics/"+name+".mp3 repeat").data(),NULL,0,NULL);
	if(ret){
		printError(name+".mp3",ret);
		return false;
	}
	return true;
}
bool Sound::playOgg(const string &name){
	return false;
}
bool Sound::playMidi(const string &name){
	//midi没法像mp3那样直接用repeat,因为repeat只适用视频流,所以需要type mpegvideo告诉MCI
	auto ret=mciSendString(("open musics/"+name+".mid type mpegvideo alias "+name).data(),NULL,0,NULL);
	ret=mciSendString(("play "+name+" repeat").data(),NULL,0,NULL);
	if(ret){
		printError(name+".mid",ret);
		return false;
	}
	return true;
}
#else
bool Sound::playWav(const string &name){return false;}
bool Sound::playMp3(const string &name){return false;}
bool Sound::playOgg(const string &name){return false;}
bool Sound::playMidi(const string &name){return false;}
#endif//__MINGW32__
bool Sound::playBGM(const string &name){
	bool ret=playMidi(name);
	if(!ret)ret=playOgg(name);
	if(!ret)ret=playMp3(name);
	if(!ret)ret=playWav(name);
	return ret;
}