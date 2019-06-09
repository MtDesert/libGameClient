#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include"Point.h"

/**摄像机*/
class GameCamera{
public:
	GameCamera();

	Point3D<int> size;//摄像机拍摄区域
	//拍摄控制
	Point3D<float> position;//摄像机中心位置
	Point3D<float> targetPosition;//目标位置
	Point3D<float> upDirection;//摄像机朝向
	//摄像机边界坐标
	float leftX()const;//最左横坐标
	float rightX()const;//最右横坐标
	float topY()const;//最上纵坐标
	float bottomY()const;//最下纵坐标
};
#endif