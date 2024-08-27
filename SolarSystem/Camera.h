#ifndef CAMERA_H
#define CAMERA_H

#include<GLFW/glfw3.h>
#include <string>
#include "Sphere.h"

struct sphereCoords {
	//Polar angle is from North (vertical)
	//Azimuth angle is horizontal from x axis
	float radius, polarAngle, azimuthAngle;
};

class Camera {
private:
	Vector3 pos;
	sphereCoords posSphere;
	const float speed = 0.2f;
	const float angleSpeed = 0.002f;
public:
	Camera();
	void update(Sphere& sphere);
	void move(std::string direction);
	Vector3 translatePos(sphereCoords coords);
};

#endif
