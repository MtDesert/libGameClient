#include"GameObject.h"
#include<stdio.h>

GameObject::GameObject():parentObject(nullptr),forceIntercept(false){}
GameObject::~GameObject(){clearSubObjects();}

//遍历子GameObject指针
#define FOR_SUB_OBJECTS(code)\
for(auto &obj:subObjects){\
	if(obj){code;}\
}

/*事件传递,代码解释如下
拦截状态=不拦截
while(有子物体时){
	拦截状态=子物体执行GameObject的事件函数
	若没有拦截,则拦截状态=子物体执行自己class的同名事件函数
	若子物体要求强行拦截,则拦截状态=真
	若拦截,则break此whlie循环
}
返回拦截状态
*/
#define EVENT_TRANSMISSION(code)\
bool intercept=false;\
SizeType objPos=subObjects.size();\
while(objPos>0){\
	--objPos;\
	auto obj=subObjects.data(objPos);\
	if(obj && *obj){\
		bool force=(*obj)->forceIntercept;\
		intercept=(*obj)->GameObject::code;\
		if(!intercept){\
			intercept=(*obj)->code;/*这里的操作有可能导致*obj的地址发生变化,故之后的代码不要再操作*obj*/\
		}\
		if(force)intercept=true;\
		if(intercept)break;\
	}\
}\
return intercept;

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
void GameObject::removeFromParentObject(){
	if(parentObject)parentObject->removeSubObject(this);
}
void GameObject::deleteFromParentObject(){
	removeFromParentObject();
	delete this;
}

void GameObject::reset(){
	FOR_SUB_OBJECTS(obj->reset())
}

bool GameObject::joystickKey(JoystickKey key,bool pressed){
	EVENT_TRANSMISSION(joystickKey(key,pressed))
}
bool GameObject::keyboardKey(Keyboard::KeyboardKey key,bool pressed){
	EVENT_TRANSMISSION(keyboardKey(key,pressed))
}
bool GameObject::mouseKey(MouseKey key, bool pressed){
	EVENT_TRANSMISSION(mouseKey(key,pressed))
}
bool GameObject::mouseMove(int x,int y){
	EVENT_TRANSMISSION(mouseMove(x,y))
}
bool GameObject::mouseWheel(int angle){
	EVENT_TRANSMISSION(mouseWheel(angle))
}
void GameObject::render()const{
	FOR_SUB_OBJECTS(obj->render());
}