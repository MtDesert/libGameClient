#ifndef GAMETABLE_H
#define GAMETABLE_H

#include"GameMenu.h"
#include"GameString.h"
#include"extern.h"

/*游戏中常用的表,表的作用是用来给玩家查询一些数据
本类只负责简单画上表格线,以及一些文字数据,不负责存储数据
*/
template<typename T>
class GameTableTemplate:public GameMenuTemplate<T>{
public:
	GameTableTemplate():itemHeight(32){}

	SizeType itemHeight;//行高
	//列相关
	virtual SizeType columnAmount()const{return 0;}//列数,间接决定了尺寸
	virtual SizeType columnWidth(uint column)const{return 0;}//各个列的宽度,在子类中实现
	//渲染表格
	virtual void renderX()const{
		auto rect=this->rectF();
		decltype(rect) rct;
		//渲染整个表格内容
		SizeType x=0,y=0,colAmount=columnAmount();
		for(y=0;y<this->renderItemAmount;++y){
			//调整渲染参数
			rct.p0.x = rect.p0.x;
			rct.p0.y = rect.p1.y-(y+1) * itemHeight;
			rct.p1.y = rct.p0.y + itemHeight;
			for(x=0;x<colAmount;++x){
				rct.p1.x = rct.p0.x + columnWidth(x);//调整渲染参数
				//渲染边框
				shapeRenderer.drawRectangle(rct,&ColorRGBA::White,nullptr);
				rct.p0.x = rct.p1.x;//调整渲染参数
			}
		}
	}
};
#endif