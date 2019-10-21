#include"GameCamera.h"

GameCamera::GameCamera(){}

float GameCamera::leftX()const{return position.x-size.x/2;}
float GameCamera::rightX()const{return position.x+size.x/2;}
float GameCamera::topY()const{return position.y+size.y/2;}
float GameCamera::bottomY()const{return position.y-size.y/2;}