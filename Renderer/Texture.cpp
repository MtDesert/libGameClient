#include"Texture.h"
//缓冲区
static DataBlock fileDataBlock;//图片文件数据缓冲
static Bitmap_32bit bitmap;//图像数据,用于传递给显卡或第三方库
#ifdef __MINGW32__
HDC Texture::deviceContext=nullptr,Texture::compatibleDeviceContex=nullptr;
static BLENDFUNCTION blendFunction={AC_SRC_OVER,0,255,AC_SRC_ALPHA};
#endif

Texture::Texture():
#ifdef __MINGW32__
hBitmap(NULL),
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
	hBitmap=nullptr;
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
	SelectObject(compatibleDeviceContex,hBitmap);
	GdiAlphaBlend(deviceContext,rect.left(),rect.bottom(),rect.width(),rect.height(),compatibleDeviceContex,0,0,rect.width(),rect.height(),blendFunction);
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