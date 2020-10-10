#include"Scene_Logo.h"
#include"Game.h"
#include"CountDown.h"

Scene_Logo::Scene_Logo():whenLogoOver(nullptr){
	addSubObject(&logoText);
}
Scene_Logo::~Scene_Logo(){}

void Scene_Logo::reset(){
	logoText.color.alpha=0;
	cdFadeTo.fadeTo(&logoText,1000,0xFF,[&](){
		cdDelay.startCountDown(1000,0,[&](){
			cdFadeTo.fadeTo(&logoText,1000,0,[&](){
				if(whenLogoOver)whenLogoOver();
			});
		});
	});
}