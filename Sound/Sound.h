#ifndef SOUND_H
#define SOUND_H

#include<string>
using namespace std;

class Sound{
public:
	//播放特定格式的音频
	bool playWav(const string &name);
	bool playMp3(const string &name);
	bool playOgg(const string &name);
	bool playMidi(const string &name);
	//播放
	bool playBGM(const string &name);//播放背景音乐
	bool playSoundEffect(const string &name);
};
#endif