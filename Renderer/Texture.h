#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __MINGW32__
#include"windows.h"
//模拟OpenGL的矩阵变换
void glPushMatrix();
void glPopMatrix();
void glTranslatef(float,float,float);
void glRotatef(float angle,float,float,float);
void glScalef(float,float,float);
#else
#include"GLES/gl.h"
#endif
#include"FileBMP.h"
#include"FilePNG.h"
#include"Pixmap.h"

/*纹理主要是指在显存中的东西,而如果在内存或者外存中我们应该叫做图像,
纹理由显卡驱动直接调度,可以进行快速渲染,目前的平台如下
Linux->OpenGL
Windows->GDI32
*/
class Texture{
#ifdef __MINGW32__
	HBITMAP hBitmap;//纹理句柄
	ColorRGBA bitmapColor;//当前纹理的颜色
	HBITMAP hBitmapShadow;//根据hBitmap推出来的影子
	ColorRGBA shadowColor;//影子的颜色,影响渲染
#else
	GLuint texture;//纹理ID
#endif//__MINGW32__
	int width,height;//纹理尺寸
public:
	//构造/析构
	Texture();
	~Texture();

	//创建/删除纹理
	void texImage2D(int width,int height,const void* pixels);
	void deleteTexture();
	//创建纹理(通过图像文件或者内存图像来创建)
	void texImage2D(const FileBMP &fileBmp);
	void texImage2D(const FilePNG &filePng);
	void texImage2D(const Pixmap<bool> &pixmap);
	void texImage2D(const Pixmap<uint8> &pixmap);
	void texImage2D(const Bitmap_32bit &bitmap);
	//创建纹理(通过各种文件名)
	void texImage2D_FileBMP(const string &filename);
	void texImage2D_FilePNG(const string &filename,WhenErrorString whenError=nullptr);
	void texImage2D_FileName(const string &filename);

	//局部绘制枚举
	enum TexCoord{
		TexCoord_Default,//默认绘制整个纹理
		TexCoord_LeftHalf,//左半部分(可用于绘制ASCII字母)
		TexCoord_RightHalf,//右半部分
		TexCoord_UpHalf,//上半部分
		TexCoord_DownHalf//下半部分
	};
	//绘制纹理(从p开始绘制,绘制大小为size,若没指定size则以本身的宽和高来作为默认size)
	void draw(const Point2D<float> &p,TexCoord coord=TexCoord_Default)const;
	void draw(const Point2D<float> &p,const Point2D<float> &size,TexCoord coord=TexCoord_Default)const;
	void draw(const Rectangle2D<float> &rect,TexCoord coord=TexCoord_Default)const;

	//纹理的几何尺寸
	int getWidth()const;//宽
	int getHeight()const;//高
	Point2D<int> size()const;//宽高
	Point2D<float> sizeF()const;//宽高
	//格式转换
	static void rect2vertex(const Rectangle2D<float> &rect,float vertex[]);

	static Texture makeSolidTexture(int width,int height,const uint32 &u32);//生成特定颜色的纹理
#ifdef __MINGW32__
	void setColor(const ColorRGBA &color);//设置纹理颜色,以便生成变色后的纹理
	static HDC deviceContext;//绘图设备
#endif
};

//文字纹理,主要用于文字渲染
class FontTexture:public Texture{
public:
	FontTexture();
	uint16 charCode;//文字编码
};

/*纹理缓冲,缓存各种纹理,为游戏提供各种支持*/
class TextureCache:public Array<Texture>{
public:
	void clearCache();
	Texture getTexture(SizeType index)const;//根据index获取纹理
};
/*二级纹理缓冲*/
class TextureCacheArray:public Array<TextureCache>{
public:
	void clearCache();
	Texture getTexture(SizeType idxA,SizeType idxB)const;//根据idxA和idxB获取纹理
};
//字符串纹理缓冲
class TextureCache_String:public Map<string,Texture>{
public:
	void clearCache();
	Texture getTexture(const string &texName)const;//根据texName获取纹理
};
#endif//TEXTURE_H