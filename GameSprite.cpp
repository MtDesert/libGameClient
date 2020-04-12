#include"GameSprite.h"
#include"ShapeRenderer.h"
#include"Game.h"

GameSprite::GameSprite():color(0xFFFFFFFF),
	bgColor(nullptr),borderColor(nullptr),
	rotateAngle(0),
	scale(1,1,1),
	anchorPoint(0.5,0.5){}
GameSprite::~GameSprite(){}

void GameSprite::setTexture(const Texture *tex){if(tex)setTexture(*tex);}
void GameSprite::setTexture(const Texture &tex){
	texture=tex;
	size.setXY(tex.getWidth(),tex.getHeight());
}
void GameSprite::setColor(const ColorRGBA &clr){
	color=clr;
#ifdef __MINGW32__
	texture.setColor(clr);
#endif
}
bool GameSprite::isMouseOnSprite()const{
	auto rect=rectF();
	auto &pos(Game::currentGame()->mousePos);
	//判断前需要进行平移
	rect.translate(position.x,position.y);
	auto parent=this->parentObject;
	while(parent){
		auto sprite=dynamic_cast<GameSprite*>(parent);
		if(sprite){
			rect.translate(sprite->position.x,sprite->position.y);
			parent=sprite->parentObject;
		}else{
			parent=nullptr;
		}
	}
	return rect.containPoint(pos.x,pos.y);
}
void GameSprite::horizontalLayout(SizeType start,SizeType spacing){
	forEachSubObj<GameSprite>([&](GameSprite &sprite){
		auto w=sprite.size.x;
		sprite.position.x = start + w/2;
		start += w + spacing;
	});
}
void GameSprite::verticalLayout(SizeType start,SizeType spacing){
	forEachSubObj<GameSprite>([&](GameSprite &sprite){
		auto h=sprite.size.y;
		sprite.position.y = start - h/2;
		start -= h + spacing;
	});
}

void GameSprite::consumeTimeSlice(){}
void GameSprite::render()const{
	glPushMatrix();//保存矩阵
	//变换
#ifdef __ANDROID__
	glScalex(scale.x,scale.y,scale.z);
	glRotatex(rotateAngle,rotation.x,rotation.y,position.z);
	glTranslatex(position.x,position.y,position.z);
#else
	glScalef(scale.x,scale.y,scale.z);
	glRotatef(rotateAngle,rotation.x,rotation.y,position.z);
	glTranslatef(position.x,position.y,position.z);
#endif//__ANDROID__
	//绘制纹理
	auto rect=rectF();
	ShapeRenderer::drawRectangle(rect,nullptr,bgColor);//画背景
	ShapeRenderer::setColor(color);
	texture.draw(rect);
	ShapeRenderer::drawRectangle(rect,borderColor,nullptr);//画边框
	//其它绘制
	renderX();//特殊绘制
	GameObject::render();//递归绘制子节点
	glPopMatrix();//恢复矩阵
}
void GameSprite::renderX()const{}

Rectangle2D<float> GameSprite::rectF()const{
	Rectangle2D<float> rect;
#ifdef __MINGW32__
	rect.p0.x = -anchorPoint.x * size.x;
	rect.p0.y = anchorPoint.y * size.y - size.y;
	rect.p1 = rect.p0 + size;
#else
	rect.p0.x=-anchorPoint.x * size.x;
	rect.p0.y= -anchorPoint.y * size.y;
	rect.p1 = rect.p0 + size;
#endif
	return rect;
}