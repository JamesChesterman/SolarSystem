#ifndef CAMERA_H
#define CAMERA_H

#include<GLFW/glfw3.h>
#include "Sphere.h"

class Camera {
private:
	Vector3 pos;
public:
	Camera();
	void setCameraPos(Vector3 newPos, Sphere sphere);

};

#endif
