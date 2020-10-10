#include"CountDown.h"
#include"Game.h"
#include"Number.h"

CountDown::CountDown():cdElapsed(0),cdRemain(0),cdTotal(0),repeatTime(1),whenCountingDown(nullptr),whenCountingDownOver(nullptr){}
CountDown_FadeTo::CountDown_FadeTo():pSprite(nullptr),srcAlpha(0),dstAlpha(0){}

void CountDown::startCountDown(CD_TYPE ms,int repeat,WhenOverType whenOver){
	cdTotal=ms;
	cdRemain=cdTotal;
	cdElapsed=0;
	repeatTime=repeat;
	whenCountingDownOver=whenOver;
	Game::currentGame()->countDownList.push_back(this);
}
bool CountDown::countingDown(CD_TYPE ms){
	//减少时间
	if(ms<=cdRemain){
		cdElapsed+=ms;
		cdRemain-=ms;
	}else{
		cdElapsed+=cdRemain;
		cdRemain=0;
	}
	//触发回调
	if(whenCountingDown)whenCountingDown(ms);//触发倒计时过程
	if(cdRemain<=0){//时间到了,触发倒计时结束后的动作
		if(whenCountingDownOver)whenCountingDownOver();
		if(repeatTime>0){//有次数限制时,按次数限制来执行
			--repeatTime;
			if(repeatTime<=0)return true;
		}else{//无次数限制时,循环执行
			cdElapsed-=cdTotal;
			cdRemain+=cdTotal;
		}
	}
	return false;//倒计时未结束
}

void CountDown_FadeTo::fadeTo(GameSprite *sprite,CD_TYPE ms,uint8 destAlpha,WhenOverType whenOver){
	//设定参数
	pSprite=sprite;
	srcAlpha=sprite->color.alpha;
	dstAlpha=destAlpha;
	//设置回调函数
	if(ms>0){
		if(srcAlpha<dstAlpha){//变大
			whenCountingDown=[&](CD_TYPE){
				pSprite->color.alpha=Number::divideRound(dstAlpha*cdElapsed + srcAlpha*cdRemain,cdElapsed+cdRemain);
				pSprite->setColor(pSprite->color);//递归设置颜色
			};
		}
	}
	startCountDown(ms,1,whenOver);
}

void CountDownList::countingDown(CD_TYPE ms){
	SizeType pos=0;
	CountDown **pCD=nullptr;
	while(pos<size()){
		pCD=data(pos);
		if(*pCD){//执行各个倒计时的事件函数
			if((*pCD)->countingDown(ms))erase(pos);
			else ++pos;//执行完毕时移除,否则继续执行
		}else{//出现空指针,移除
			erase(pos);
		}
	}
}