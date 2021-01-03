#ifndef COLORCMYK_H
#define COLORCMYK_H

#include"ColorRGB.h"
//CMYK系列,用于打印,即Cyan青Magenta洋红Yellow黄,主要用于确定墨水颜色的含量以确定打印时候的出墨量

struct ColorCMY{//自定义的颜色结构,和RGB为互补关系
	ColorCMY(uint8 c=0,uint8 m=0,uint8 y=0);
	ColorCMY(const ColorRGB &color);

	uint8 cyan,magenta,yellow;
	void fromRGB(const ColorRGB &color);
	void toRGB(ColorRGB &color)const;
};
struct ColorCMYK{//标准的CMYK结构
	ColorCMYK(DECIMAL c=0,DECIMAL m=0,DECIMAL y=0,DECIMAL k=0);
	ColorCMYK(const ColorRGB &rgb);

	DECIMAL cyan,magenta,yellow,black;

	void fromRGB(const ColorRGB &rgb);
	void toRGB(ColorRGB &rgb)const;
};
#endif