#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"typedef.h"
#include"Array.h"
#include"Keyboard.h"

//这是所有的手柄按键,根据需求可能有更多的按键
#define JOYSTICK_ALLKEY(MACRO)\
	MACRO(Up)\
	MACRO(Down)\
	MACRO(Left)\
	MACRO(Right)\
	MACRO(Select)\
	MACRO(Start)\
	MACRO(A)\
	MACRO(B)\
	MACRO(C)\
	MACRO(X)\
	MACRO(Y)\
	MACRO(Z)\
	MACRO(L1)\
	MACRO(L2)\
	MACRO(R1)\
	MACRO(R2)\

#define MOUSE_ALLKEY(MACRO)\
	MACRO(LeftButton)\
	MACRO(MiddleButton)\
	MACRO(RightButton)\

/** @brief GameObject 游戏中的物体
 * 游戏中的场景、图层、精灵等都可以看作物体,当然也可以不继承此类
 * 此类的作用主要是输入事件的传递,比如键盘事件,鼠标事件,手柄事件等
 * 此类还可以基于时间片来进行响应(类似于timer机制),时钟其实也是一种输入
 * 此类可以传递输出事件,比如渲染,本类只会简单把渲染的调用传递下去
 */
class GameObject{
public:
	GameObject();
	virtual ~GameObject();

	//手柄按键枚举
#define JOYSTICK_ENUM(key) Joystick_##key,
	enum JoystickKey{
		JOYSTICK_ALLKEY(JOYSTICK_ENUM)
		Amount_JoystickKey
	};

	//鼠标按键枚举
#define MOUSE_ENUM(key) Mouse_##key,
	enum MouseKey{
		MOUSE_ALLKEY(MOUSE_ENUM)
		Amount_MouseKey
	};

	//保存子类的容器,主要用于事件传递
	//警告,subObject相当于树叉,如果把让它形成环状(比如addSubObject(this);)会出现严重的后果,除非你知道你自己在干什么
	GameObject *parentObject;//父物体
	Array<GameObject*> subObjects;//子物体
	bool forceIntercept;//是否进行事件拦截

	void addSubObject(GameObject *subObj,bool addFront=false);
	void reAddSubObject(GameObject *subObj,bool addFront=false);
	void removeSubObject(GameObject *subObj);
	void clearSubObjects();
	void deleteSubObject(GameObject *subObj);

	void removeFromParentObject();
	void deleteFromParentObject();

	//如需要响应事件,请在子类重写virtual函数
	virtual void reset();//重启,相当于游戏机的reset按钮
	//传递手柄按键事件
	virtual bool joystickKey(JoystickKey key,bool pressed);
	virtual bool keyboardKey(Keyboard::KeyboardKey key,bool pressed);
	virtual bool mouseKey(MouseKey key,bool pressed);
	//鼠标有移动事件和滚轮事件
	virtual bool mouseMove(int x,int y);//鼠标移动
	virtual bool mouseWheel(int angle);//单位为角度

	//消耗时间片,某些object可能会有基于时间片的运动
	virtual void consumeTimeSlice();
	//渲染,递归进行渲染
	virtual void render()const;

	//对subObjects中所有的T类型的对象进行callback操作
	template<typename T>
	void forEachSubObj(function<void(T &obj)> callback){
		if(!callback)return;
		for(auto &pObj:subObjects){
			auto pType=dynamic_cast<T*>(pObj);
			if(pType){
				callback(*pType);
			}
		}
	}
};
#endif