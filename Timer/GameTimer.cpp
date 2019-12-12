#include "GameTimer.h"

TimeSlice::TimeSlice():timeSlice(0),minSlice(0),maxSlice(0),sprite(nullptr){}

void TimeSlice::addTimeSlice(uint msec){
	timeSlice+=msec;
	if(timeSlice>maxSlice)timeSlice=maxSlice;//限制最大值
	if(timeSlice>=minSlice){//时间片足够时,消耗时间片
		timeSlice-=minSlice;
		if(sprite)sprite->consumeTimeSlice();
	}
}

void TimeSliceList::pushTimeSlice(GameSprite *sprite, uint minSlice, uint maxSlice){
	TimeSlice slice;
	slice.sprite=sprite;
	slice.minSlice=minSlice;
	slice.maxSlice=maxSlice;
	this->push_back(slice);
}

static GameSprite *toRemoveSprite=nullptr;
static bool func(const TimeSlice &slice){return slice.sprite==toRemoveSprite;}
void TimeSliceList::removeTimeSlice(GameSprite *sprite){
	toRemoveSprite=sprite;
	this->remove_if(func);
}

void TimeSliceList::addTimeSlice(uint msec){
	for(auto &timer:*this){
		timer.addTimeSlice(msec);
	}
}