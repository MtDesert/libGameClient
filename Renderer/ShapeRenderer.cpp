#include"ShapeRenderer.h"
#ifdef __MINGW32__
HDC ShapeRenderer::deviceContext=nullptr;
ColorRGBA ShapeRenderer::color;
static HPEN pen=nullptr;
static HBRUSH brush=nullptr;
#else
#include"GLES/gl.h"
#endif

ShapeRenderer ShapeRenderer::shapeRenderer;
ShapeRenderer::ShapeRenderer():edgeColor(nullptr),fillColor(nullptr),texture(0){}
ShapeRenderer::~ShapeRenderer(){}

void ShapeRenderer::setColor(const ColorRGBA &color,bool useAlpha){
	if(useAlpha)shapeRenderer.color=color;
	else{
		auto old=shapeRenderer.color.alpha;
		shapeRenderer.color=color;
		shapeRenderer.color.alpha=old;
	}
#ifdef __MINGW32__
	DeleteObject(pen);
	DeleteObject(brush);
	pen=CreatePen(PS_SOLID,1,color.toRGB());
	brush=CreateSolidBrush(color.toRGB());
	SelectObject(deviceContext,pen);
	SelectObject(deviceContext,brush);
#else
	glColor4ub(color.red,color.green,color.blue,
		useAlpha ? color.alpha : shapeRenderer.color.alpha);
#endif
}
void ShapeRenderer::drawRectangle(const Rectangle2D<numType> &rect,const ColorRGBA *border,const ColorRGBA *background){
	shapeRenderer.edgeColor=border;
	shapeRenderer.fillColor=background;
	shapeRenderer.drawRectangle(rect);
}

//画点
void ShapeRenderer::drawPoint(numType x,numType y)const{
	numType vertex[2];
	vertex[0]=x;
	vertex[1]=y;
	drawPoints(vertex,1);
}
void ShapeRenderer::drawPoint(const Point2D<numType> &p)const{
	drawPoints(&p.x,1);
}
void ShapeRenderer::drawPoints(const numType vertex[],int n)const{
#ifdef __MINGW32__
#else
	if(edgeColor){
		glBindTexture(GL_TEXTURE_2D,0);
		setColor(*edgeColor,true);
		glVertexPointer(2,GL_FLOAT,0,vertex);
		glDrawArrays(GL_POINTS,0,n);
	}
#endif
}

//画线
#define SHAPERENDERER_DRAWLINE(x0,y0,x1,y1) \
numType vertex[4];\
vertex[0]=x0;\
vertex[1]=y0;\
vertex[2]=x1;\
vertex[3]=y1;\
drawLines(vertex,1);

void ShapeRenderer::drawLine(numType x0,numType y0,numType x1,numType y1)const{
	SHAPERENDERER_DRAWLINE(x0,y0,x1,y1)
}
void ShapeRenderer::drawLine(const Point2D<numType> p0,const Point2D<numType> p1)const{
	SHAPERENDERER_DRAWLINE(p0.x,p0.y,p1.x,p1.y)
}
void ShapeRenderer::drawLine(const Line2D<numType> &line)const{
	SHAPERENDERER_DRAWLINE(line.p0.x,line.p0.y,line.p1.x,line.p1.y)
}

void ShapeRenderer::drawLines(const numType vertex[],int n)const{
#ifdef __MINGW32__
#else
	if(edgeColor){
		glBindTexture(GL_TEXTURE_2D,0);
		setColor(*edgeColor,false);
		glVertexPointer(2,GL_FLOAT,0,vertex);
		glDrawArrays(GL_LINES,0,n*2);
	}
#endif
}

//折线
void ShapeRenderer::drawBrokenLine(const numType vertex[],int n)const{
#ifdef __MINGW32__
#else
	if(edgeColor){
		glBindTexture(GL_TEXTURE_2D,0);
		setColor(*edgeColor,false);
		glVertexPointer(2,GL_FLOAT,0,vertex);
		glDrawArrays(GL_LINE_STRIP,0,n*2);
	}
#endif
}

//三角形
#define SHAPERENDERER_DRAW_TRIANGLE(x0,y0,x1,y1,x2,y2) \
numType vertex[6];\
vertex[0]=x0;\
vertex[1]=y0;\
vertex[2]=x1;\
vertex[3]=y1;\
vertex[4]=x2;\
vertex[5]=y2;\
drawTriangle(vertex);

void ShapeRenderer::drawTriangle(numType x0,numType y0,numType x1,numType y1,numType x2,numType y2)const{
	SHAPERENDERER_DRAW_TRIANGLE(x0,y0,x1,y1,x2,y2)
}
void ShapeRenderer::drawTriangle(const Point2D<numType> p0,const Point2D<numType> p1,const Point2D<numType> p2)const{
	SHAPERENDERER_DRAW_TRIANGLE(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y)
}
void ShapeRenderer::drawTriangle(const Triangle2D<numType> &triangle)const{
	SHAPERENDERER_DRAW_TRIANGLE(
		triangle.p0.x,triangle.p0.y,
		triangle.p1.x,triangle.p1.y,
		triangle.p2.x,triangle.p2.y)
}
void ShapeRenderer::drawTriangle(const numType vertex[])const{
	drawPolygen(vertex,3);
}

//画矩形
#define SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)\
numType vertex[8];\
vertex[0]=vertex[6]=x0;\
vertex[1]=vertex[3]=y0;\
vertex[2]=vertex[4]=x1;\
vertex[5]=vertex[7]=y1;\
drawRectangle(vertex);

void ShapeRenderer::drawRectangle(numType x0,numType y0,numType x1,numType y1)const{
	SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)
}
void ShapeRenderer::drawRectangle(const Point2D<numType> &p0,const Point2D<numType> &p1)const{
	SHAPERENDERER_DRAW_RECTANGLE(p0.x,p0.y,p1.x,p1.y)
}
void ShapeRenderer::drawRectangle(const Rectangle2D<numType> &rect)const{
	SHAPERENDERER_DRAW_RECTANGLE(rect.p0.x,rect.p0.y,rect.p1.x,rect.p1.y)
}
void ShapeRenderer::drawRectangle(const numType vertex[])const{
	drawPolygen(vertex,4);
}

//多边形
void ShapeRenderer::drawPolygen(const numType vertex[],int n)const{
#ifdef __MINGW32__
	if(fillColor){
		setColor(*fillColor);
		//绘制多边形
		POINT ptArr[n];
		for(int i=0;i<n;++i){
			ptArr[i].x=vertex[i*2];
			ptArr[i].y=vertex[i*2+1];
		}
		Polygon(deviceContext,ptArr,n);
	}
	if(edgeColor){
		setColor(*edgeColor);
		MoveToEx(deviceContext,vertex[0],vertex[1],NULL);
		for(int i=1;i<=n;++i){
			auto idx=i%n;
			LineTo(deviceContext,vertex[idx*2],vertex[idx*2+1]);
		}
	}
#else
	glBindTexture(GL_TEXTURE_2D,texture);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	if(fillColor){//绘制底色或纹理
		setColor(*fillColor,false);
		glDrawArrays(GL_TRIANGLE_FAN,0,n);
	}
	if(edgeColor){//绘制边框线
		setColor(*edgeColor,false);
		glDrawArrays(GL_LINE_LOOP,0,n);
	}
#endif
}