#include"Scene_Logo.h"
#include"Game.h"
#include"CountDown.h"

//流程控制
enum Status{
	FadeIn,
	Delay,
	FadeOut,
	StatusOver
};

Scene_Logo::Scene_Logo():whenLogoOver(nullptr){
	addSubObject(&logoText);
}
Scene_Logo::~Scene_Logo(){
	Game::currentGame()->timeSliceList.removeTimeSlice(this);
}

void Scene_Logo::reset(){
	logoText.color.alpha=0;
	cdFadeTo.fadeTo(&logoText,1000,0xFF,[&](){
		cdDelay.startCountDown(1000,[&](){
			cdFadeTo.fadeTo(&logoText,1000,0,[&](){
				if(whenLogoOver)whenLogoOver();
			});
		});
	});
}