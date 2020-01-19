#include"GameObject.h"
#include<stdio.h>

GameObject::GameObject():parentObject(nullptr){}
GameObject::~GameObject(){clearSubObjects();}

//遍历子GameObject指针
#define FOR_SUB_OBJECTS(code)\
for(auto obj:subObjects){\
	if(obj){code;}\
}

#define EVENT_TRANSMISSION(code)\
bool intercept=false;\
SizeType objPos=subObjects.size();\
while(objPos>0){\
	--objPos;\
	auto obj=subObjects.data(objPos);\
	if(obj && *obj){\
		intercept=code;\
		if(intercept)break;\
	}\
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
void GameObject::deleteSubObject(GameObject *subObj){
	if(subObj){
		removeSubObject(subObj);
		delete subObj;
	}
}

void GameObject::reset(){
	FOR_SUB_OBJECTS(obj->reset())
}
bool GameObject::joystickKey(JoystickKey key,bool pressed){
	EVENT_TRANSMISSION((*obj)->joystickKey(key,pressed))
	return intercept;
}
bool GameObject::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	EVENT_TRANSMISSION((*obj)->keyboardKey(key,pressed))
	return intercept;
}
bool GameObject::mouseKey(MouseKey key, bool pressed){
	EVENT_TRANSMISSION((*obj)->mouseKey(key,pressed))
	return intercept;
}
bool GameObject::mouseMove(int x,int y){
	EVENT_TRANSMISSION((*obj)->mouseMove(x,y))
	return intercept;
}
bool GameObject::mouseWheel(int angle){
	EVENT_TRANSMISSION((*obj)->mouseWheel(angle))
	return intercept;
}
void GameObject::addTimeSlice(uint msec){
	FOR_SUB_OBJECTS(obj->addTimeSlice(msec))
}
void GameObject::render()const{
	FOR_SUB_OBJECTS(obj->render());
}