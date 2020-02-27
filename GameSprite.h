#ifndef GAMESPRITE_H
#define GAMESPRITE_H

#include"GameObject.h"
#include"Texture.h"
#include"ColorRGB.h"

/*游戏中的精灵,可以用于表现人物,物体等各种东西*/
class GameSprite:public GameObject{
public:
	GameSprite();
	virtual ~GameSprite();

	//显示内容
	Texture texture;//精灵的纹理,基本上每个精灵都有纹理,如果2D游戏要变换显示的内容,简单改变此值就好
	ColorRGBA color;//精灵可以有颜色,可以通过变色来区分
	Point2D<SizeType> size;//精灵的大小,没有纹理的情况下可以考虑使用此值
	const ColorRGBA *bgColor,*borderColor;//背景色与边框色,在有值的情况下进行绘制
	//几何变量
	Point3D<int> position;//位置(用来表现在屏幕上的位置)
	double rotateAngle;//旋转角度
	Point3D<int> rotation;//旋转方向
	Point3D<int> scale;//缩放
	Point2D<float> anchorPoint;//锚点(百分比)

	void setTexture(const Texture &tex);

	//以下函数有可能子类差异化
	virtual void consumeTimeSlice();
	virtual void render()const;//渲染texture
	virtual void renderX()const;//子类特殊的渲染方式
	virtual Point2D<float> posF()const;//返回精灵的位置,值由成员变量推断
	virtual Rectangle2D<float> rectF()const;//返回精灵在屏幕上的矩形区域,值由成员变量推断
};
#endif