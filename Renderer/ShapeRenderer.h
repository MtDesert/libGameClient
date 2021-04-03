#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include"Triangle.h"
#include"Rectangle.h"
#include"Circle.h"
#include"Cube.h"
#include"Line.h"

#include"ColorRGB.h"

//图形渲染器,利用现有的绘图引擎(比如OpenGL)来绘制图形
//不同的绘图引擎可能会有差异
class ShapeRenderer{
public:
	//类型定义
	//typedef int numType;
	typedef float numType;
	typedef Point2D<numType> Pt2;
	typedef Point3D<numType> Pt3;
private:
	//各种绘图函数,mode为图形类别,表示画的是什么
	//vertex顶点数据,d维度,n数量
	void draw(uint mode,const numType vertex[],int d,int n)const;
	void drawPoints(const numType vertex[],int d,int n)const;//点
	void drawLines(const numType vertex[],int d,int n)const;//线
	void drawPolygen(const numType vertex[],int n,int d)const;//多边形
protected:
	uint tex2D;//2维纹理,渲染用
public:
	ShapeRenderer();
	virtual ~ShapeRenderer();

	//选项
	static bool fillMode;//填充模式,true表示会对绘制的多边形进行填充

	//设置当前绘制的颜色(OpenGL的话则是传给状态机)	
	static void setColor(const ColorRGBA &color,uint8 alpha);
	static void setColor(const ColorRGBA &color);

	//画点
	void drawPoint(numType x,numType y)const;
	void drawPoint(numType x,numType y,numType z)const;
	void drawPoint(const Pt2 &p)const;
	void drawPoint(const Pt3 &p)const;
	//画直线2D
	void drawLine2D(numType x0,numType y0,numType x1,numType y1)const;
	void drawLine2D(const Pt2 &p0,const Pt2 &p1)const;
	void drawLine2D(const Line2D<numType> &line)const;
	//画直线3D
	void drawLine3D(numType x0,numType y0,numType z0,numType x1,numType y1,numType z1)const;
	void drawLine3D(const Pt3 &p0,const Pt3 &p1)const;
	void drawLine3D(const Line3D<numType> &line)const;
	//画折线
	void drawBrokenLine2D(const numType vertex[],int n)const;
	void drawBrokenLine3D(const numType vertex[],int n)const;

	//2D多边形
	void drawPolygen2D(const numType vertex[],int n)const;/*多边形*/
	void drawPolygen2D(const Point2D<numType> vertex[],int n)const;/*多边形*/
	void drawTriangle2D(const Triangle2D<numType> &triangle)const;/*三角形*/
	void drawRectangle(numType x0,numType y0,numType x1,numType y1)const;/*矩形*/
	void drawRectangle(const Pt2 &p0,const Pt2 &p1)const;/*矩形*/
	void drawRectangle(const Rectangle2D<numType> &rect)const;/*矩形*/
	void drawCircle(const Circle<numType> &circle)const;/*圆*/

	//3D多边形
	void drawPolygen3D(const numType vertex[],int n)const;
	void drawPolygen3D(const Point3D<numType> vertex[],int n)const;
	//画三角形
	void drawTriangle3D(const Triangle3D<numType> &triangle)const;
	//立方体3D
	void drawCubeLines(const CubeF &cube)const;
	void drawCubePlanes(const CubeF &cube)const;
};
#endif