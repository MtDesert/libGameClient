#include"GameText.h"
#include"Charset.h"
#include"extern.h"

GameText::GameText():charAmountPerLine(16),renderLineStart(0),renderLineAmount(0){}
GameText::GameText(const string &text):GameString(text){}
GameText::~GameText(){}

void GameText::setString(const string &str){
	GameString::setString(str);
	updateRenderParameter();
	renderLineStart=0;
}
uint GameText::stringWidth()const{return charSize.x*min(charAmountPerLine,byteAmount);}
Point2D<float> GameText::sizeF()const{
	auto sz=lineStartList.size();
	sz=min(sz,renderLineAmount);
	return Point2D<float>(stringWidth(),charSize.y*sz);
}
void GameText::renderX()const{
	ShapeRenderer::setColor(color);
	rect=rectF();
	point2D=rect.p0;
	point2D.y=rect.p1.y-charSize.y;
	size2D.y=charSize.y;
	//开始计算
	size_t line=0,from=0,to=0;
	for(auto itr=lineStartList.begin();itr!=lineStartList.end();++itr){
		//过滤出需要渲染的行
		if(line<renderLineStart){
			++line;
			continue;
		}
		if(line-renderLineStart>=renderLineAmount)break;
		//获取渲染范围
		auto itrNext=itr;++itrNext;
		from=*itr;
		to=(itrNext!=lineStartList.end()?*itrNext:arrayCharAttr.size());
		if(renderCharAmount<to){
			renderString(from,renderCharAmount-from);
			break;
		}else{
			renderString(from,to-from);
		}
		//准备下一行渲染
		++line;
		point2D.setXY(rect.p0.x,point2D.y-charSize.y);
	}
}

void GameText::setLineCharAmount(uint lineAmount, uint charAmountPerLine){
	if(lineAmount){
		this->renderLineAmount=lineAmount;
	}
	if(charAmountPerLine){
		this->charAmountPerLine=charAmountPerLine;
	}
	if(lineAmount || charAmountPerLine){
		updateRenderParameter();
	}
}
void GameText::readyPrint(){
	renderLineStart=0;
	renderCharAmount=0;
}
bool GameText::printNextLine(){
	//检查合法性
	auto maxVal=lineStartList.size();
	if(maxVal<renderLineAmount)return false;
	//改变渲染起始行
	if(renderLineStart < maxVal - renderLineAmount){
		++renderLineStart;
		return true;
	}
	return false;
}
bool GameText::printNextChar(){
	++renderCharAmount;
	//检查是否填满
	auto sz=arrayCharAttr.size();
	if(renderCharAmount>=sz){//字符串显示到了最后一行
		renderCharAmount=sz;
		return true;
	}
	//字符串没显示到最后一行
	auto idx = lineStartList.indexOf(renderCharAmount);
	if(idx>=0){
		return renderLineStart + renderLineAmount <= (uint)idx;
	}
	return false;
}

void GameText::updateRenderParameter(){
	//在这里立刻计算行数,并将行信息存入数组
	lineStartList.clear();
	uint from=0,count=0,charLen=0;
	auto len=arrayCharAttr.size();
	for(uint i=0;i<len;++i){
		charLen = arrayCharAttr.data(i)->isAscii ? 1 : 2;
		count += charLen;
		//这里判断是否需要换行
		if(count>charAmountPerLine){//超过行上限,要换
			lineStartList.push_back(from);//保存起来
			from=i;//确定新的起点
			count=charLen;//确定新的长度
		}
	}
	//保存最后一行,即使没达到上限
	lineStartList.push_back(from);
}