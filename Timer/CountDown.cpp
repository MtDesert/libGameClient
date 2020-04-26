#include"CountDown.h"
#include"Game.h"
#include"Number.h"

CountDown::CountDown():cdElapsed(0),cdRemain(0),whenCountingDown(nullptr),whenCountingDownOver(nullptr){}
CountDown_FadeTo::CountDown_FadeTo():pSprite(nullptr),srcAlpha(0),dstAlpha(0){}

void CountDown::startCountDown(uint ms,WhenOverType whenOver){
	cdRemain=ms;
	cdElapsed=0;
	whenCountingDownOver=whenOver;
	Game::currentGame()->countDownList.push_back(this);
}
bool CountDown::countingDown(uint ms){
	//减少时间
	if(ms<=cdRemain){//时间充足
		cdElapsed+=ms;//逝去时间
		cdRemain-=ms;//剩余时间
	}else{//时间不足
		cdElapsed+=cdRemain;
		cdRemain=0;
	}
	//触发回调
	if(whenCountingDown)whenCountingDown(ms);//触发倒计时过程
	if(cdRemain<=0){//时间到了,触发倒计时结束后的动作
		if(whenCountingDownOver)whenCountingDownOver();
		return true;//倒计时结束
	}
	return false;//倒计时未结束
}

void CountDown_FadeTo::fadeTo(GameSprite *sprite,uint ms,uint8 destAlpha,WhenOverType whenOver){
	//设定参数
	pSprite=sprite;
	srcAlpha=sprite->color.alpha;
	dstAlpha=destAlpha;
	//设置回调函数
	if(ms>0){
		if(srcAlpha<dstAlpha){//变大
			whenCountingDown=[&](uint){
				pSprite->color.alpha=Number::divideRound(dstAlpha*cdElapsed + srcAlpha*cdRemain,cdElapsed+cdRemain);
				pSprite->setColor(pSprite->color);//递归设置颜色
			};
		}
	}
	startCountDown(ms,whenOver);
}

void CountDownList::countingDown(uint ms){
	SizeType pos=0;
	CountDown **pCD=nullptr;
	while(pos<size()){
		pCD=data(pos);
		if(!(*pCD) || (*pCD)->countingDown(ms)){//执行完毕,移除
			erase(pos);
		}else{//不移除
			++pos;
		}
	}
}