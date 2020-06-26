#ifndef FILEMP3_H
#define FILEMP3_H

#include"DataBlock.h"

//MP3标记,相当于文件头
struct FileMP3_ID3V2:public DataBlock{
	//结构
	DATABLOCK_CUSTOM_TYPE(TagID,uint32)
	DATABLOCK_CUSTOM_TYPE(TagVersionMajor,uint8)
	DATABLOCK_CUSTOM_TYPE(TagVersionMinor,uint8)
	DATABLOCK_CUSTOM_TYPE(Flags,uint8)
	DATABLOCK_CUSTOM_TYPE(SizeOfTag,uint32)

	string tagID()const;
	string tagVersion()const;
	uint32 tagSize()const;
};

//MP3的ID3V2帧,保存MP3的一些相关信息
struct FileMP3_ID3V2_Frame:public DataBlock{
	SizeType parseData();
	//结构
	DATABLOCK_CUSTOM_TYPE(FrameType,uint32)
	DATABLOCK_CUSTOM_TYPE(Size,uint32)
	DATABLOCK_CUSTOM_TYPE(Flag,uint16)

	string frameName()const;
	void setFrameName(const string &name);
};

//MP3音频帧,保存有音频数据
struct FileMP3_AudioFrame:public DataBlock{
	//枚举
	enum MPEGversionID{
		MpegVer2_5,//非官方
		MpegVer_Unknown,
		MpegVer2,
		MpegVer1,//大多数MP3是这个值
	};
	enum Layer{
		LayerUnknown,
		Layer3,//大多数MP3是这个值
		Layer2,
		Layer1
	};
	enum BitRateIndex{
		BitRate_Free,
		BitRate_32,
		BitRate_40,
		BitRate_48,
		BitRate_56,
		BitRate_64,
		BitRate_80,
		BitRate_96,
		BitRate_112,
		BitRate_128,
		BitRate_160,
		BitRate_192,
		BitRate_224,
		BitRate_256,
		BitRate_320,
		BitRate_Bad,
	};
	enum SamplingRateFrequencyIndex{
		SamplingRateFrequency_44100,
		SamplingRateFrequency_48000,
		SamplingRateFrequency_32000,
		SamplingRateFrequency_Unknown,
	};
	enum Channel{
		Channel_Stereo,
		Channel_JointStereo,
		Channel_Dual,
		Channel_Mono,
	};
	enum ModeExtension{
		ModeExtension_Off,
		ModeExtension_Intensity,
		ModeExtension_MS,
		ModeExtension_Intensity_MS,
	};
	enum Emphasis{
		Emphasis_None,
		Emphasis_50_15,
		Emphasis_Unknown,
		Emphasis_CCIT_J17,
	};
	//数据
	DATABLOCK_CUSTOM_TYPE(Synchronizer,uint16)//11111111 111.....
	DATABLOCK_CUSTOM_TYPE(MPEGversionID,uint8)//版本号,一般是MPEG1
	DATABLOCK_CUSTOM_TYPE(Layer,uint8)//层,一般是Layer3
	DATABLOCK_CUSTOM_BOOL(CRCprotect)//0保护,1不保护
	DATABLOCK_CUSTOM_TYPE(BitRateIndex,uint8)//bit率
	DATABLOCK_CUSTOM_TYPE(SamplingRateFrequencyIndex,uint8)//采样频率
	DATABLOCK_CUSTOM_BOOL(Padding)//填充,影响帧长计算
	DATABLOCK_CUSTOM_BOOL(PrivateBit)//私有位,一般不用
	DATABLOCK_CUSTOM_TYPE(Channel,uint8)//声道
	DATABLOCK_CUSTOM_TYPE(ModeExtension,uint8)//模式扩展(JointStereo专用)
	DATABLOCK_CUSTOM_BOOL(Copyright)//0无版权,1有版权
	DATABLOCK_CUSTOM_BOOL(Original)//0原版的复制品,1原版
	DATABLOCK_CUSTOM_TYPE(Emphasis,uint8)//重点

	DATABLOCK_CUSTOM_TYPE(CRC,uint16)//循环冗余校验(CRCprotect保护时存在)

	SizeType parseData();
	//验证
	bool isSynchronizer()const;//是否同步头
	//特征
	static uint16 bitRate(uint8 index);
	static uint16 sampleRate(uint8 index);
};

//MP3文件结构
struct FileMP3:public DataBlock{
	SizeType parseData();
	//结构
	FileMP3_ID3V2 fileMP3_ID3V2;
};

#endif //FILEMP3_H