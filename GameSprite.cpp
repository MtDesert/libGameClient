#include"GameSprite.h"
#include"ShapeRenderer.h"
#include"Game.h"
#include"Number.h"

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
	forEachSubObj<GameSprite>([&](GameSprite &sprite){
		sprite.setColor(color);
	});
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
//布局
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

decltype(GameSprite::position) GameSprite::screenPosition()const{
	decltype(GameSprite::position) ret(position);
	auto parent=parentObject;
	while(parent){
		auto sprite=dynamic_cast<GameSprite*>(parent);
		if(sprite){
			ret = ret + sprite->position;
			parent=sprite->parentObject;
		}else{
			parent=nullptr;
		}
	}
	return ret;
}
void GameSprite::setScreenPosition(const decltype(GameSprite::position) &pos){position = position + (pos-screenPosition());}
void GameSprite::setScreenPosition_Corner(bool right,bool top){
	decltype(position) pos(Game::resolution);
	pos=pos/2;
	//顶点修正
	if(!right)pos.x=-pos.x;
	if(!top)pos.y=-pos.y;
	//x锚点修正
	if(right)pos.x -= size.x*(1-anchorPoint.x);
	else pos.x += size.x*anchorPoint.x;
	//y锚点修正
	if(top)pos.y -= size.y*(1-anchorPoint.y);
	else pos.y += size.y*anchorPoint.y;
	setScreenPosition(pos);
}

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
	if(bgColor){//绘制单色背景
		ShapeRenderer::setColor(color);//主要传递透明度
		ShapeRenderer::drawRectangle(rect,nullptr,bgColor);//画背景
	}
	//绘制纹理
	ShapeRenderer::setColor(color);
	texture.draw(rect);
	if(borderColor){//绘制边框
		ShapeRenderer::setColor(color);//主要传递透明度
		ShapeRenderer::drawRectangle(rect,borderColor,nullptr);//画边框
	}
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