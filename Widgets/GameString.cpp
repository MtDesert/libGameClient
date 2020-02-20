#include"GameString.h"
#include"Charset.h"
#include"Game.h"
#include"extern.h"

//静态变量
Charset GameString::charset;
FontTextureCache GameString::fontTextureCache;
//渲染用的变量
static uint8 u8;//存放单字节的字符
static uint16 u16;//存放双字节的字符
static SizeType w;//渲染时候计算字符长度用的变量

GameString::GameString():charSize(16,32),renderCharAmount(0),byteAmount(0){}
GameString::GameString(const string &str):GameString(){
	setString(str);
}
GameString::~GameString(){}

void GameString::setString(const string &str,bool translate){
	//获取字符及其特征
	auto block=charset.newString(translate ? Game::currentGame()->translate(str) : str.data());
	setRawString((char*)block.dataPointer);
}
void GameString::setRawString(const string &str){
	//获取字符及其特征
	DataBlock block(str.data(),str.length()+1);
	auto charAmount=Charset::charAmount((const char*)block.dataPointer,charset.destCharset);
	//申请缓冲
	arrayCharAttr.setSize(charAmount,true);
	renderCharAmount=charAmount;
	//生成纹理
	for(uint pos=0,i=0;pos<block.dataLength;++pos,++i){
		auto attr=arrayCharAttr.data(i);
		//开始判断
		block.get_uint8(pos,u8);
		if(u8=='\0'){//字符串结束
			byteAmount=pos;
			break;
		}
		attr->isAscii=(u8<0x80);
		if(attr->isAscii){//ASCII字母
			u16=u8;
		}else{//可能是汉字
			block.get_uint16(pos,u16);
			++pos;
		}
		attr->tex=fontTextureCache.renderCharCode(u16);
	}
	//调整尺寸
	size.setXY(stringWidth(),charSize.y);
}
SizeType GameString::stringWidth()const{return charSize.x*byteAmount;}

void GameString::renderX()const{
	shapeRenderer.setColor(color);
	point2D=rectF().p0;
	size2D.y=charSize.y;
	//开始计算
	renderString(0,renderCharAmount);
}

void GameString::renderString(uint from,uint amount)const{
	for(decltype(amount) i=0;i<amount;++i){
		//获取文字纹理
		auto attr=arrayCharAttr.data(from+i);
		if(!attr)break;
		//根据是否ASCII来进行渲染
		if(attr->isAscii){
			w=size2D.x=charSize.x;//确定宽度
			attr->tex.draw(point2D,size2D,Texture::TexCoord_LeftHalf);//渲染
		}else{
			w=size2D.x=charSize.x*2;//确定宽度
			attr->tex.draw(point2D,size2D);
		}
		point2D.x+=w;//确定下一个字的绘制位置
	}
}