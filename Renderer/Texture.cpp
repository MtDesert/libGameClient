#include"Texture.h"
#include<math.h>
//缓冲区
static DataBlock fileDataBlock;//图片文件数据缓冲
static Bitmap_32bit bitmap;//图像数据,用于传递给显卡或第三方库

#ifdef __MINGW32__
#include"ShapeRenderer.h"
HDC Texture::deviceContext=nullptr;//绘图设备
static BLENDFUNCTION blendFunction={AC_SRC_OVER,0,255,AC_SRC_ALPHA};
//模拟OpenGL的驱动
static const XFORM orgXForm = {1,0,0,1,0,0};
static XFORM currentXForm = orgXForm;

void glPushMatrix(){
	SaveDC(Texture::deviceContext);
}
void glPopMatrix(){
	RestoreDC(Texture::deviceContext,-1);
	GetWorldTransform(Texture::deviceContext,&currentXForm);
}
void glTranslatef(float x,float y,float z){
	currentXForm.eDx+=x;
	currentXForm.eDy+=-y;
	//执行顺序是scale,rotate,translate
	SetWorldTransform(Texture::deviceContext,&currentXForm);
}
void glScalef(float x,float y,float z){
	currentXForm.eM11*=x;
	currentXForm.eM22*=y;
}
void glRotatef(float angle,float x,float y,float z){
	/*auto arc=angle*PI/180;
	currentXForm.eM11=cos(arc);
	currentXForm.eM22=sin(arc);*/
}
#endif

Texture::Texture():
#ifdef __MINGW32__
hBitmap(NULL),bitmapColor(0xFF,0xFF,0xFF),hBitmapShadow(NULL),shadowColor(0xFF,0xFF,0xFF),
#else
texture(0),
#endif
width(0),height(0){}
Texture::~Texture(){}
FontTexture::FontTexture():charCode(0){}

static float vertex[]={0,0,0,0,0,0,0,0};
static float texCoord_Default[]  ={0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0};
static float texCoord_LeftHalf[] ={0.0,0.0, 0.5,0.0, 0.5,1.0, 0.0,1.0};
static float texCoord_RightHalf[]={0.5,0.0, 1.0,0.0, 1.0,1.0, 0.5,1.0};
static float texCoord_UpHalf[]   ={0.0,0.5, 1.0,0.5, 1.0,1.0, 0.0,1.0};
static float texCoord_DownHalf[] ={0.0,0.0, 1.0,0.0, 1.0,0.5, 0.0,0.5};

//创建纹理
void Texture::texImage2D(int width,int height,const void *pixels){
#ifdef __MINGW32__
	deleteTexture();
	hBitmap=CreateBitmap(width,height,1,32,pixels);
#else
	if(!glIsTexture(texture)){//防止多次申请从而导致原texture值丢失
		glGenTextures(1,&texture);//申请纹理序号
	}
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
#endif//__MINGW32__
	this->width=width;
	this->height=height;
}
void Texture::deleteTexture(){
#ifdef __MINGW32__
	DeleteObject(hBitmap);
	DeleteObject(hBitmapShadow);
	hBitmap=hBitmapShadow=nullptr;
#else
	//glIsTexture(texture);在驱动有问题的情况下返回值会有问题,这里的处理方法是直接删除
	glDeleteTextures(1,&texture);
	texture=0;
#endif//__MINGW32__
}
//创建纹理(根据不同的类)
void Texture::texImage2D(const FileBMP &fileBmp){
	fileBmp.decodeTo(bitmap);
	texImage2D(bitmap);
}
void Texture::texImage2D(const FilePNG &filePng){
	filePng.decodeTo(bitmap);
	texImage2D(bitmap);
}
void Texture::texImage2D(const Pixmap<bool> &pixmap){
	bitmap.coordinateType=Bitmap_32bit::CoordinateType_Math;
	pixmap.toBitmap(bitmap);
	texImage2D(bitmap);
}
void Texture::texImage2D(const Pixmap<uint8> &pixmap){
	pixmap.toBitmap(bitmap);
	texImage2D(bitmap);
}
void Texture::texImage2D(const Bitmap_32bit &bitmap){
	texImage2D(bitmap.getWidth(),bitmap.getHeight(),bitmap.dataPointer);
}
//创建纹理(根据文件名)
void Texture::texImage2D_FileBMP(const string &filename){
	FileBMP fileBmp;
	fileBmp.set(fileDataBlock.loadFile(filename));
	fileBmp.parseData();
	texImage2D(fileBmp);
}
void Texture::texImage2D_FilePNG(const string &filename,WhenErrorString whenError){
	FilePNG filePng;
	filePng.set(fileDataBlock.loadFile(filename,whenError));
	filePng.parseData();
	texImage2D(filePng);
}
void Texture::texImage2D_FileName(const string &filename){
	//根据扩展名来识别
	auto pos=filename.find_last_of(".");
	auto suffix=filename.substr(pos);
	auto len=suffix.length();
	if(len!=4)return;//目前图片的扩展名包括点共4个
	for(decltype(len) i=0;i<len;++i){//转换成小写再比对
		suffix[i]=tolower(suffix[i]);
	}
	//开始比对
	if(suffix==".bmp"){
		texImage2D_FileBMP(filename);
	}else if(suffix==".png"){
		texImage2D_FilePNG(filename);
	}
}

void Texture::draw(const Point2D<float> &p,TexCoord coord)const{
	draw(p,sizeF()-Point2D<float>(1,1),coord);
}
void Texture::draw(const Point2D<float> &p,const Point2D<float> &size,TexCoord coord)const{
	draw(Rectangle2D<float>(p,p+size),coord);
}
void Texture::draw(const Rectangle2D<float> &rect,TexCoord coord)const{
#ifdef __MINGW32__
	HDC cdc=CreateCompatibleDC(deviceContext);
	if(bitmapColor==ColorRGBA::White){//本色演出
		SelectObject(cdc,hBitmap);
	}else{//给图片绘制上影子
		SelectObject(cdc,hBitmapShadow);
	}
	GdiAlphaBlend(deviceContext,rect.left(),rect.bottom(),rect.width(),rect.height(),cdc,0,0,rect.width(),rect.height(),{AC_SRC_OVER,0,ShapeRenderer::color.alpha,AC_SRC_ALPHA});
	DeleteDC(cdc);
#else
	if(!glIsTexture(texture))return;
	glBindTexture(GL_TEXTURE_2D,texture);
	//顶点数组
	rect2vertex(rect,vertex);
	glVertexPointer(2,GL_FLOAT,0,vertex);
	//纹理坐标数组
#define TEXTURE_TEXCOORD(name) case TexCoord_##name:glTexCoordPointer(2,GL_FLOAT,0,texCoord_##name);break;
	switch(coord){
		TEXTURE_TEXCOORD(Default)
		TEXTURE_TEXCOORD(LeftHalf)
		TEXTURE_TEXCOORD(RightHalf)
		TEXTURE_TEXCOORD(UpHalf)
		TEXTURE_TEXCOORD(DownHalf)
#undef TEXTURE_TEXCOORD
		default:;
	}
	//绘制
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
#endif//__MINGW32__
}

int Texture::getWidth()const{return width;}
int Texture::getHeight()const{return height;}
Point2D<int> Texture::size()const{return Point2D<int>(width,height);}
Point2D<float> Texture::sizeF()const{return Point2D<float>(width,height);}

void Texture::rect2vertex(const Rectangle2D<float> &rect,float vertex[]){
	vertex[0]=vertex[6]=rect.p0.x;
	vertex[1]=vertex[3]=rect.p0.y;
	vertex[2]=vertex[4]=rect.p1.x;
	vertex[5]=vertex[7]=rect.p1.y;
}

Texture Texture::makeSolidTexture(int width, int height, const uint32 &u32){
	Texture tex;
	bitmap.newBitmap(width,height);
	bitmap.fillColor(u32);
	tex.texImage2D(bitmap);
	return tex;
}
#ifdef __MINGW32__
void Texture::setColor(const ColorRGBA &color){
	//调整颜色
	if(bitmapColor==color)return;//无变化
	bitmapColor=color;
	//判断是否要生成影子
	if(color==ColorRGBA::White)return;//全白为本色,不用生成影子
	if(!(shadowColor==ColorRGBA::White) && bitmapColor==shadowColor)return;//和影色一样,则不需要再生成影子
	//生成影子
	if(!bitmap.newBitmap(width,height))return;//申请空间
	if(!GetBitmapBits(hBitmap,bitmap.dataLength,bitmap.dataPointer)==bitmap.dataLength)return;//获取图像数据
	//开始调色
	uint32 u32=0;
	ColorRGBA tmpColor;
	for(int y=0;y<height;++y){
		for(int x=0;x<width;++x){
			bitmap.getColor(x,y,u32);//读取
			if(u32 & 0xFF000000){//非透明,使用color
				tmpColor.fromBGRA(u32);
				tmpColor=tmpColor.darkColor(color);
				u32=tmpColor.toBGRA() | 0xFF000000;//不要改变alpha
			}else{
				u32=0;
			}
			bitmap.setColor(x,y,u32);//写入
		}
	}
	//写回图形驱动
	DeleteObject(hBitmapShadow);
	hBitmapShadow=CreateBitmap(width,height,1,32,bitmap.dataPointer);
}
#endif

void TextureCache::clearCache(){
	for(auto &item:*this){
		item.deleteTexture();
	}
	clear();
}
Texture TextureCache::getTexture(SizeType index)const{
	auto tex=data(index);
	return tex ? *tex : Texture();
}
void TextureCacheArray::clearCache(){
	for(auto &cache:*this){
		cache.clearCache();
	}
	clear();
}
Texture TextureCacheArray::getTexture(SizeType idxA,SizeType idxB)const{
	auto arr=data(idxA);
	return arr ? arr->getTexture(idxB) : Texture();
}
void TextureCache_String::clearCache(){
	for(auto &item:*this){
		item.value.deleteTexture();
	}
	clear();
}
Texture TextureCache_String::getTexture(const string &texName)const{
	auto tex=value(texName);
	return tex ? *tex : Texture();
}