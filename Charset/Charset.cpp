#include"Charset.h"
#include<iconv.h>
#include<string.h>

#define STR_NAME(name) #name,
static const char* strCharset[]={//静态字符表
	CHARSET_ALL(STR_NAME)
};
//缓冲区
static DataBlock targetBlock;

Charset::Charset():srcCharset(UTF8),destCharset(GBK){
	strCharset[UTF8]="UTF-8";//Windows的iconv -l命令查出"UTF-8"而非"UTF8",这会影响iconv_open的返回值
}
Charset::~Charset(){
	targetBlock.memoryFree();
}

DataBlock Charset::newString(const char *srcStr)const{
	newString(srcStr,srcCharset,destCharset);
	return targetBlock;
}

SizeType Charset::charAmount(const char *str,EnumCharset charset){
	SizeType ret=0,pos=0,len=0,length=strlen(str);
	if(str){
		uchar byte=str[pos];
		while(byte){
			len=(byte<0x80);//测字符长度
			if(len!=1){//非ASCII的符号需要另外转换
				switch(charset){
					case GB2312:case GBK:len=2;break;//GB系列的汉字默认为2字节
					case GB18030:
						if(byte>=0x81 && byte<=0x82)len=4;//可能有4个字节的情况
						else len=2;
					break;
					case UTF8://UTF8要从首字节的最高位开始检测
						len=0;
						while(byte & 0x80){++len;byte<<=1;}//检测1的连续个数
					break;
					default:len=1;//未知的就先按1
				}
			}
			if(pos+len>length)break;//保证字符完整
			++ret;//统计
			pos+=len;//下一个
			byte=str[pos];
		}
	}
	return ret;
}

static SizeType prefixLen,bytesPerChar;
//转换过程变量
static iconv_t convert;
#ifdef __MINGW32__
static const char *fromStr;
#else
static char *fromStr;
#endif
static char *toStr;
static SizeType fromLen,toLen;

bool Charset::newString(const char *srcStr,EnumCharset fromCharset,EnumCharset toCharset){
	if(!srcStr || fromCharset>=AmountOf_EnumCharset || toCharset>=AmountOf_EnumCharset)return false;//防非法输入
	prefixLen=0,bytesPerChar=1;//前缀字符个数,每个字符可能的个数
	switch(toCharset){//根据目标字符集确定每个字可能的最长长度
		case UNICODE:prefixLen=2;bytesPerChar=2;break;//iconv转换后自带2字节的头
		case GB2312:case GBK:bytesPerChar=2;break;
		case UTF8:bytesPerChar=3;break;
		case GB18030:bytesPerChar=4;break;
		default:;
	}
	//获得目标可能的大小,调整缓冲区空间
	toLen = prefixLen + charAmount(srcStr,fromCharset) * bytesPerChar + 1;//测字符数,确定需要的转换空间
	targetBlock.memoryAllocate(toLen,true);
	toLen=targetBlock.dataLength;
	//申请成功就开始转换
	if(targetBlock.dataPointer){
		//准备转换,变量赋值
		fromStr = (char*)srcStr;
		fromLen = strlen(srcStr);//测字节数
		toStr = (char*)targetBlock.dataPointer;
		//iconv转换的完整过程
		convert=iconv_open(strCharset[toCharset],strCharset[fromCharset]);
		iconv(convert,&fromStr,&fromLen,&toStr,&toLen);
		iconv_close(convert);
		//结束符号
		targetBlock.dataPointer[targetBlock.dataLength-toLen]='\0';
	}
	return true;
}