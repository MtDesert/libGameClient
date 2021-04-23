#include"ShapeRenderer.h"
#include"Number.h"

#include"GL/gl.h"

bool ShapeRenderer::fillMode=false;
typedef ShapeRenderer::numType numType;
//常用纹理坐标,请参考ShapeRenderer::TexCoord
static numType allTexCoords[5][8]={
	{0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0},
	{0.0,0.0, 0.5,0.0, 0.5,1.0, 0.0,1.0},
	{0.5,0.0, 1.0,0.0, 1.0,1.0, 0.5,1.0},
	{0.0,0.0, 1.0,0.0, 1.0,0.5, 0.0,0.5},
	{0.0,0.5, 1.0,0.5, 1.0,1.0, 0.0,1.0},
};

ShapeRenderer::ShapeRenderer():tex2D(0){}
ShapeRenderer::~ShapeRenderer(){}

void ShapeRenderer::draw(uint mode, const numType vertex[],int d,int n,TexCoord texCoord)const{
	glBindTexture(GL_TEXTURE_1D,0);
	glBindTexture(GL_TEXTURE_2D,tex2D);
	//确定纹理坐标
	if(tex2D){
		glTexCoordPointer(2,GL_FLOAT,0,allTexCoords[texCoord]);
	}
#ifdef __i386
	switch(mode){
		case GL_LINE_LOOP:{
			glBegin(mode);
			for(int i=0;i<n;++i){
				switch(d){
					case 2:glVertex2f(vertex[i*2],vertex[i*2+1]);break;
					case 3:glVertex3f(vertex[i*3],vertex[i*3+1],vertex[i*3+2]);break;
					case 4:glVertex4f(vertex[i*4],vertex[i*4+1],vertex[i*4+2],vertex[i*4+3]);break;
				}
			}
			glEnd();
			return;
		}break;
	}
#endif
	glVertexPointer(d,GL_FLOAT,0,vertex);
	glDrawArrays(mode,0,n);
}
void ShapeRenderer::drawPoints(const numType vertex[],int d,int n)const{draw(GL_POINTS,vertex,d,n);}
void ShapeRenderer::drawLines(const numType vertex[],int d,int n)const{draw(GL_LINES,vertex,d,n);}
void ShapeRenderer::drawPolygen(const numType vertex[], int d, int n,TexCoord texCoord)const{
	draw((tex2D||fillMode) ? GL_TRIANGLE_FAN : GL_LINE_LOOP,vertex,d,n,texCoord);
}

void ShapeRenderer::setColor(const ColorRGBA &color,uint8 alpha){glColor4ub(color.red,color.green,color.blue,alpha);}
void ShapeRenderer::setColor(const ColorRGBA &color){glColor4ub(color.red,color.green,color.blue,color.alpha);}
//画点
void ShapeRenderer::drawPoint(numType x,numType y)const{
	numType vtx[]={x,y};
	drawPoints(vtx,1,2);
}
void ShapeRenderer::drawPoint(numType x,numType y,numType z)const{
	numType vtx[]={x,y,z};
	drawPoints(vtx,1,3);
}
void ShapeRenderer::drawPoint(const Pt2 &p)const{
	numType vtx[]={p.x,p.y};
	drawPoints(vtx,1,2);
}
void ShapeRenderer::drawPoint(const Pt3 &p)const{
	numType vtx[]={p.x,p.y,p.z};
	drawPoints(vtx,1,3);
}

//画线
#define SHAPERENDERER_DRAWLINE(x0,y0,x1,y1) \
numType vertex[]={x0,y0,x1,y1};\
drawLines(vertex,2,2);

void ShapeRenderer::drawLine2D(numType x0,numType y0,numType x1,numType y1)const{
	SHAPERENDERER_DRAWLINE(x0,y0,x1,y1)
}
void ShapeRenderer::drawLine2D(const Pt2 &p0,const Pt2 &p1)const{
	SHAPERENDERER_DRAWLINE(p0.x,p0.y,p1.x,p1.y)
}
void ShapeRenderer::drawLine2D(const Line2D<numType> &line)const{
	SHAPERENDERER_DRAWLINE(line.p0.x,line.p0.y,line.p1.x,line.p1.y)
}
//3D直线
#define SHAPERENDERER_DRAWLINE_3D(x0,y0,z0,x1,y1,z1) \
numType vertex[]={x0,y0,z0,x1,y1,z1};\
drawLines(vertex,3,2);

void ShapeRenderer::drawLine3D(numType x0,numType y0,numType z0,numType x1,numType y1,numType z1)const{
	SHAPERENDERER_DRAWLINE_3D(x0,y0,z0,x1,y1,z1)
}
void ShapeRenderer::drawLine3D(const Pt3 &p0,const Pt3 &p1)const{
	SHAPERENDERER_DRAWLINE_3D(p0.x,p0.y,p0.z,p1.x,p1.y,p1.z)
}
void ShapeRenderer::drawLine3D(const Line3D<numType> &line)const{
	SHAPERENDERER_DRAWLINE_3D(line.p0.x,line.p0.y,line.p0.z,line.p1.x,line.p1.y,line.p1.z)
}

//折线
void ShapeRenderer::drawBrokenLine2D(const numType vertex[],int n)const{draw(GL_LINE_STRIP,vertex,2,n);}
void ShapeRenderer::drawBrokenLine3D(const numType vertex[],int n)const{draw(GL_LINE_STRIP,vertex,3,n);}

//多边形
void ShapeRenderer::drawPolygen2D(const numType vertex[],int n)const{drawPolygen(vertex,2,n);}
void ShapeRenderer::drawPolygen2D(const Point2D<numType> vertex[],int n)const{drawPolygen2D((numType*)vertex,n);}
void ShapeRenderer::drawPolygen3D(const numType vertex[],int n)const{drawPolygen(vertex,3,n);}
void ShapeRenderer::drawPolygen3D(const Point3D<numType> vertex[],int n)const{drawPolygen3D((numType*)vertex,n);}

void ShapeRenderer::drawCircle(const Circle<numType> &circle)const{
	drawPolygen2D(circle.vertex,circle.segment);
}

//三角形
void ShapeRenderer::drawTriangle2D(const Triangle2D<numType> &triangle)const{drawPolygen2D((numType*)triangle.vertex,3);}
void ShapeRenderer::drawTriangle3D(const Triangle3D<numType> &triangle)const{drawPolygen3D((numType*)triangle.vertex,3);}

//画矩形
#define SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)\
numType vertex[]={x0,y0,x1,y0,x1,y1,x0,y1};\
drawPolygen(vertex,2,4,texCoord);

void ShapeRenderer::drawRectangle(numType x0,numType y0,numType x1,numType y1,TexCoord texCoord)const{
	SHAPERENDERER_DRAW_RECTANGLE(x0,y0,x1,y1)
}
void ShapeRenderer::drawRectangle(const Pt2 &p0,const Pt2 &p1,TexCoord texCoord)const{
	SHAPERENDERER_DRAW_RECTANGLE(p0.x,p0.y,p1.x,p1.y)
}
void ShapeRenderer::drawRectangle(const Rectangle2D<numType> &rect,TexCoord texCoord)const{
	SHAPERENDERER_DRAW_RECTANGLE(rect.p0.x,rect.p0.y,rect.p1.x,rect.p1.y)
}
//立方体
void ShapeRenderer::drawCubeLines(const CubeF &cube)const{
	CubeF::setCubeF(cube);
	for(int i=0;i<cube.AmountOfLine;++i){
		drawPolygen3D(cube.getLineVertexs(i),2);
	}
}
void ShapeRenderer::drawCubePlanes(const CubeF &cube)const{
	CubeF::setCubeF(cube);
	for(int i=0;i<cube.AmountOfPlane;++i){
		drawPolygen3D(cube.getPlaneVertexs(i),4);
	}
}