#include "GameTimer.h"

TimeSlice::TimeSlice():timeSlice(0),minSlice(0),maxSlice(0),object(nullptr){}

void TimeSlice::addTimeSlice(uint msec){
	timeSlice+=msec;
	if(timeSlice>maxSlice)timeSlice=maxSlice;//限制最大值
	if(timeSlice>=minSlice){//时间片足够时,消耗时间片
		timeSlice-=minSlice;
		if(object)object->consumeTimeSlice();
	}
}

void TimeSliceList::pushTimeSlice(GameObject *object, uint minSlice, uint maxSlice){
	TimeSlice slice;
	slice.object=object;
	slice.minSlice=minSlice;
	slice.maxSlice=maxSlice;
	this->push_back(slice);
}
void TimeSliceList::removeTimeSlice(GameObject *object){
	remove_if([&](const TimeSlice &slice){
		return slice.object==object;
	});
}
void TimeSliceList::addTimeSlice(uint msec){
	for(auto &timer:*this){
		timer.addTimeSlice(msec);
	}
}