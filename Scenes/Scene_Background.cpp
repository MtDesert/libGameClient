#include "Scene_Background.h"

Scene_Background::Scene_Background(){}

void Scene_Background::setBackground(const string &filename){
	texture.texImage2D_FilePNG(filename);
	setTexture(texture);
}