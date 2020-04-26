#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include"typedef.h"
#include"Array.h"
#include"GameSprite.h"

#define COUNTDOWN_PARA uint ms

/*倒计时,用于执行一些需要延迟操作的内容*/
class CountDown{
public:
	CountDown();

	uint cdElapsed,cdRemain;//倒计时毫秒数(已消耗的,剩余的)
	//倒计时响应
	typedef function<void()> WhenOverType;
	function<void(uint ms)> whenCountingDown;//倒计时过程中需要做的反应
	WhenOverType whenCountingDownOver;//倒计时结束后需要做的事情

	void startCountDown(uint ms,WhenOverType whenOver=nullptr);//开始ms毫秒的倒计时
	bool countingDown(uint ms);//执行倒计时,返回是否执行结束
};

/*倒计时-淡入淡出*/
class CountDown_FadeTo:public CountDown{
public:
	CountDown_FadeTo();

	GameSprite *pSprite;//要操作的精灵
	uint8 srcAlpha,dstAlpha;//源alpha值和目标alpha值

	//让精灵pSprite在ms毫秒后的透明度渐变为dstAlpha
	void fadeTo(GameSprite *sprite,uint ms,uint8 dstAlpha,WhenOverType whenOver=nullptr);
};

/*精灵倒计时列表*/
class CountDownList:public Array<CountDown*>{
public:
	void countingDown(uint ms);//执行倒计时,返回是否执行结束
};
#endif//COUNTDOWN_H