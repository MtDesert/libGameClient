#include"BitmapFont.h"
#include"Charset_GB2312.h"

//在设计游戏的时候可以统一字体大小,请根据实际情况修改
#define WIDTH 16
#define HEIGHT 32
static uint engGlyphSize=WIDTH*HEIGHT/8;//英文字形字节数
static uint chsGlyphSize=engGlyphSize*2;//中文字形字节数

//构造/析构函数
BitmapFont::BitmapFont(){
	charPixmap.colorMap.insert(false,0x00000000);//底色
	charPixmap.colorMap.insert(true,0xFFFFFFFF);//字体色
}
BitmapFont::~BitmapFont(){}
BitmapFont_Ascii::BitmapFont_Ascii(){
	charPixmap.newData(WIDTH*2,HEIGHT);//宽度乘以2是OpenGL在纹理不为正方形时,绘制会出现问题
}
BitmapFont_Ascii::~BitmapFont_Ascii(){
	charPixmap.deleteData();
}
BitmapFont_GB2312::BitmapFont_GB2312(){
	charPixmap.newData(WIDTH*2,HEIGHT);
}
BitmapFont_GB2312::~BitmapFont_GB2312(){
	charPixmap.deleteData();
}

//各种渲染函数
bool BitmapFont_Ascii::renderCharCode(uint8 charCode){
	//获取字形索引
	uint8 index,scale=WIDTH/8;
	if(!Charset_GB2312::charIndex_Ascii(charCode,index))return false;
	//获取字形
	auto charData=charBlock.subDataBlock(index*engGlyphSize,engGlyphSize);
	if(charData.dataLength!=engGlyphSize)return false;
	charPixmap.fill(0);//开始绘图
	for(int y=0;y<HEIGHT;++y){
		for(int x=0;x<WIDTH;++x){
			index= y*scale + x/8;
			if(charData.dataPointer[index] & ( 1<<(x%8) )){
				charPixmap.setColor(x,y,1);
			}
		}
	}
	return true;
}
bool BitmapFont_GB2312::renderCharCode(uint16 charCode){
	uint16 index,scale=charPixmap.getWidth()/8;
	DataBlock *block=nullptr;
	//查询字符表
	if(Charset_GB2312::charIndex_Symbol(charCode,index)){//符号区
		block=&symbolBlock;
	}else if(Charset_GB2312::charIndex_lv1Chinese(charCode,index)){//1级汉字
		block=&lv1Chinese;
	}else if(Charset_GB2312::charIndex_lv2Chinese(charCode,index)){//2级汉字
		block=&lv2Chinese;
	}else{//无字符
		return false;
	}
	//获取字形数据
	auto charData=block->subDataBlock(index*chsGlyphSize,chsGlyphSize);
	if(charData.dataLength!=chsGlyphSize)return false;
	charPixmap.fill(0);
	auto w=charPixmap.getWidth(),h=charPixmap.getHeight();
	for(uint y=0;y<h;++y){
		for(uint x=0;x<w;++x){
			index = y*scale + x/8;
			if(charData.dataPointer[index] & ( 1<<(x%8) )){
				charPixmap.setColor(x,y,1);
			}
		}
	}
	return true;
}