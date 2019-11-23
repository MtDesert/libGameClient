#ifndef GAMETIMER_H
#define GAMETIMER_H

#include"GameSprite.h"

//时间片,用于控制精灵的运动,让精灵在特定的时间间隔做特定的动作
class TimeSlice{
public:
	TimeSlice();

	uint timeSlice,minSlice,maxSlice;//目前累计的时间,执行动作的最小时间,可累计的最大时间
	GameSprite *sprite;//目标精灵,当时间片足够的时候,就消耗时间片执行sprite::consumeTimeSlice()
	void addTimeSlice(uint usec);//增加时间片
};

//时间片列表
class TimeSliceList:public List<TimeSlice>{
public:
	void pushTimeSlice(GameSprite *sprite,uint minSlice,uint maxSlice);//给精灵添加时间片
	void removeTimeSlice(GameSprite *sprite);//移除精灵的时间片

	void addTimeSlice(uint usec);//给列表中的时间片增加时间,由引擎的时间系统调用
};

#endif