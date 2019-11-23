#include"GameObject.h"
#include<stdio.h>

GameObject::GameObject():parentObject(nullptr){}
GameObject::~GameObject(){clearSubObjects();}

#define FOR_SUB_OBJECTS(code)\
for(auto obj:subObjects){\
	if(obj){code;}\
}

void GameObject::addSubObject(GameObject *subObj,bool addFront){
	if(!subObj)return;
	addFront ? subObjects.push_front(subObj) : subObjects.push_back(subObj);
	subObj->parentObject=this;
}
void GameObject::removeSubObject(GameObject *subObj){
	if(!subObj)return;
	subObjects.remove(subObj);
	subObj->parentObject=nullptr;
}
void GameObject::clearSubObjects(){
	FOR_SUB_OBJECTS(obj->parentObject=nullptr)
	subObjects.clear();
}

void GameObject::reset(){
	FOR_SUB_OBJECTS(obj->reset())
}
void GameObject::joystickKey(JoystickKey key,bool pressed){
	FOR_SUB_OBJECTS(obj->joystickKey(key,pressed))
}
void GameObject::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	FOR_SUB_OBJECTS(obj->keyboardKey(key,pressed))
}
void GameObject::mouseKey(MouseKey key, bool pressed){
	FOR_SUB_OBJECTS(obj->mouseKey(key,pressed))
}
void GameObject::mouseMove(int x,int y){
	FOR_SUB_OBJECTS(obj->mouseMove(x,y))
}
void GameObject::mouseWheel(int angle){
	FOR_SUB_OBJECTS(obj->mouseWheel(angle))
}
void GameObject::addTimeSlice(uint usec){
	FOR_SUB_OBJECTS(obj->addTimeSlice(usec))
}
void GameObject::render()const{
	FOR_SUB_OBJECTS(obj->render());
}