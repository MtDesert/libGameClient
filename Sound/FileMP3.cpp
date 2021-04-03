#include"FileMP3.h"
#include"Number.h"

#define MP3_TAG_LEN 10
#define MP3_FRAME_LEN 10
#define FRAME_NAME_SIZE 4

//MP3标头
bool FileMP3_ID3V2::getTagID(uint32 &value)const{
	uint32 val;
	if(get_uint32(0,val)){
		value = val & 0x00FFFFFF;
		return true;
	}
	return false;
}
bool FileMP3_ID3V2::setTagID(const uint32 &value){
	auto val=value & 0x00FFFFFF;
	return set_uint8(0,val) && set_uint8(1,val>>8) && set_uint8(2,val>>16);
}

DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2,TagVersionMajor,uint8,3,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2,TagVersionMinor,uint8,4,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2,Flags,uint8,5,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2,SizeOfTag,uint32,6,true)

string FileMP3_ID3V2::tagID()const{return"";}
string FileMP3_ID3V2::tagVersion()const{return"";}
uint32 FileMP3_ID3V2::tagSize()const{
	uint32 ret=0;
	uint8 val;
	for(int i=0;i<4;++i){
		if(get_uint8(6+i,val)){
			ret += (uint32)val<<((3-i)*7);
		}
	}
	return ret;
}
//MP3帧
SizeType FileMP3_ID3V2_Frame::parseData(){
	uint32 size;
	if(getSize(size)){
		dataLength = MP3_FRAME_LEN + size;
	}
	return dataLength;
}
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2_Frame,FrameType,uint32,0,true)
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2_Frame,Size,uint32,4,false)
DATABLOCK_CUSTOM_TYPE_CPP(FileMP3_ID3V2_Frame,Flag,uint16,8,true)

string FileMP3_ID3V2_Frame::frameName()const{
	uint32 type;
	if(getFrameType(type)){
		return string((const char*)&type,FRAME_NAME_SIZE);
	}else return string();
}
void FileMP3_ID3V2_Frame::setFrameName(const string &name){
	if(name.length()!=FRAME_NAME_SIZE)return;
	uint32 *u32=(uint32*)name.data();
	setFrameType(*u32);
}

//MP3音频帧
bool FileMP3_AudioFrame::getSynchronizer(uint16 &value)const{
	uint8 u0,u1;
	if(get_uint8(0,u0) && getByte(1,u1,0xE0)){
		value = u1<<8 | u0;
		return true;
	}return false;
}
bool FileMP3_AudioFrame::getBitRateIndex(uint8 &value)const{
	uint8 u=0;
	if(getByte(2,u,0xF0)){
		value = u>>4;
		return true;
	}return false;
}
bool FileMP3_AudioFrame::getSamplingRateFrequencyIndex(uint8 &value)const{
	uint8 u=0;
	if(getByte(2,u,0x0C)){
		value = u>>2;
		return true;
	}return false;
}
DATABLOCK_CUSTOM_BOOL_CPP(FileMP3_AudioFrame,Padding,2,1)

SizeType FileMP3_AudioFrame::parseData(){
	if(isSynchronizer()){//必须验证同步头
		uint8 brIndex,srIndex;
		bool padding;
		if(getBitRateIndex(brIndex) && getSamplingRateFrequencyIndex(srIndex) && getPadding(padding)){
			int bitRate=FileMP3_AudioFrame::bitRate(brIndex);
			int sampleRate=FileMP3_AudioFrame::sampleRate(srIndex);
			dataLength = Number::divideRound(144*bitRate*1000,sampleRate) + (padding?1:0);
			//printf("bit sample %u %u === %lu\n",bitRate,sampleRate,dataLength);
		}
	}else{
		dataLength=0;
	}
	return dataLength;
}
bool FileMP3_AudioFrame::isSynchronizer()const{
	uint16 syn;
	if(getSynchronizer(syn)){
		return syn==0xE0FF;
	}return false;
}
uint16 FileMP3_AudioFrame::bitRate(uint8 index){
	switch(index){
#define CASE(num) case BitRate_##num:return num;
		CASE(32)CASE(40)CASE(48)CASE(56)CASE(64)CASE(80)CASE(96)
		CASE(112)CASE(128)CASE(160)CASE(192)CASE(224)CASE(256)CASE(320)
#undef CASE
		default:return 0;
	}
}
uint16 FileMP3_AudioFrame::sampleRate(uint8 index){
	switch(index){
#define CASE(num) case SamplingRateFrequency_##num:return num;
		CASE(44100)CASE(48000)CASE(32000)
#undef CASE
		default:return 0;
	}
}

//MP3
SizeType FileMP3::parseData(){
	//头部
	if(dataLength<MP3_TAG_LEN)return dataLength;
	subDataBlock(0,MP3_TAG_LEN,fileMP3_ID3V2);
	//printf("tag len %u(%.8X)\n",fileMP3_ID3V2.tagSize(),fileMP3_ID3V2.tagSize());
	//ID3V2的各个子帧数据
	SizeType pos=MP3_TAG_LEN,len=MP3_TAG_LEN+fileMP3_ID3V2.tagSize();
	FileMP3_ID3V2_Frame frame;
	uint32 frameType;
	while(pos<len){//解析帧数据
		subDataBlock(pos,dataLength-pos,frame);
		pos+=frame.parseData();
		//debug
		frame.getFrameType(frameType);
		if(frameType==0)break;
		//printf("%.8X name=%s addr=%.8X len=%lu\n",frameType,frame.frameName().data(),(uint)(pos-frame.dataLength),frame.dataLength);
	};
	//音频数据
	pos=len;
	len=dataLength;
	FileMP3_AudioFrame audioFrame;
	while(pos<len){
		subDataBlock(pos,dataLength-pos,audioFrame);
		audioFrame.parseData();
		//搜寻下一帧
		if(audioFrame.dataLength){
			//printf("audio %.8X\n",(uint)pos);
			pos += audioFrame.dataLength;
		}else{
			//printf("skip %.8X\n",(uint)pos);
			++pos;
		}
	}
	return dataLength;
}
