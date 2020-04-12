#include"Scene_Logo.h"
#include"Game.h"

//流程控制
enum Status{
	FadeIn,
	Delay,
	FadeOut,
	StatusOver
};

Scene_Logo::Scene_Logo():status(0),whenLogoOver(nullptr){
	addSubObject(&logoText);
}
Scene_Logo::~Scene_Logo(){
	Game::currentGame()->timeSliceList.removeTimeSlice(this);
}

void Scene_Logo::reset(){
	logoText.color.alpha=0;
	status=FadeIn;
	auto &sliceList=Game::currentGame()->timeSliceList;
	sliceList.removeTimeSlice(this);
	sliceList.pushTimeSlice(this,20,40);
}
void Scene_Logo::consumeTimeSlice(){
	auto &alpha=logoText.color.alpha;
	switch(status){
		case FadeIn:
			logoText.color.alpha+=5;
			if(alpha>=255){
				status=Delay;
				countDown=0;
			}
		break;
		case Delay:
			countDown-=10;
			if(countDown<=0){
				status=FadeOut;
			}
		break;
		case FadeOut:
			alpha-=5;
			if(alpha<=0){
				status=StatusOver;
				if(whenLogoOver)whenLogoOver();
			}
		break;
	}
}