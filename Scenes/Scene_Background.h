#ifndef SCENE_BACKGROUND_H
#define SCENE_BACKGROUND_H

#include"GameScene.h"
//只有一个背景的场景
class Scene_Background:public GameScene{
public:
	Scene_Background();

	void setBackground(const string &filename);//以filename作为背景
};
#endif//SCENE_BACKGROUND_H