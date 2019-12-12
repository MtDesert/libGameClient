#include"FontTextureCache.h"

FontTextureCache::FontTextureCache(){}
FontTextureCache::~FontTextureCache(){deleteCache();}

//检索函数
static uint16 targetCharCode=0;
static bool findTarget(const FontTexture &tex){return tex.charCode==targetCharCode;}
Texture FontTextureCache::renderCharCode(uint16 character){
	FontTexture tex;
	targetCharCode=character;
	auto val=textureList.data(findTarget);
	if(val){
		tex=*val;
	}else{//找不到,应该生成新的
		if(character<0x80){
			bitmapFontAscii.renderCharCode(character);//绘制文字,绘制的结果在内存中
			tex.texImage2D(bitmapFontAscii.charPixmap);//生成纹理,此时数据在显存中
		}else{
			bitmapFontGb2312.renderCharCode(character);//绘制文字,绘制的结果在内存中
			tex.texImage2D(bitmapFontGb2312.charPixmap);//生成纹理,此时数据在显存中
		}
		textureList.push_back(tex);
	}
	return tex;
}

//清除纹理
static void delTexture(FontTexture &tex){tex.deleteTexture();}
void FontTextureCache::clearCache(){
	textureList.forEach(delTexture);
	textureList.clear();
}
void FontTextureCache::deleteCache(){
	textureList.forEach(delTexture);
	textureList.deleteData();
}