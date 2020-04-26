#include"GameString.h"
#include"Charset.h"
#include"Game.h"

//静态变量
Charset GameString::charset;
FontTextureCache GameString::fontTextureCache;
static Point2D<float> point2D,size2D;

GameString::GameString():charSize(16,32),renderCharAmount(0),byteAmount(0){}
GameString::GameString(const string &str):GameString(){
	setString(str);
}
GameString::~GameString(){}

void GameString::setColor(const ColorRGBA &color){
	GameSprite::setColor(color);
#ifdef __MINGW32__
	for(auto &attr:arrayCharAttr){
		attr.tex.setColor(color);
	}
#endif
}
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
	//渲染用的变量
	uint8 u8;//存放单字节的字符
	uint16 u16;//存放双字节的字符
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
	ShapeRenderer::setColor(color);//设置颜色
	renderString(0,renderCharAmount,rectF().p0);//开始渲染
}
void GameString::insertTexture(const Texture &tex,SizeType pos){
	CharAttr attr={false,tex};
	arrayCharAttr.insert(pos,attr);
	//更新外观
	byteAmount+=2;
	++renderCharAmount;
	size.setXY(stringWidth(),charSize.y);
}

void GameString::renderString(uint from,uint amount,const Point2D<float> &pos2D)const{
	point2D=pos2D;
	size2D.setXY(0,charSize.y);
	for(decltype(amount) i=0;i<amount;++i){
		//获取文字纹理
		auto attr=arrayCharAttr.data(from+i);
		if(!attr)break;
		//根据是否ASCII来进行渲染
		if(attr->isAscii){
			size2D.x=charSize.x;//确定宽度
			attr->tex.draw(point2D,size2D,Texture::TexCoord_LeftHalf);//渲染
		}else{
			size2D.x=charSize.x*2;//确定宽度
			attr->tex.draw(point2D,size2D);
		}
		point2D.x += size2D.x;//确定下一个字的绘制位置
	}
}