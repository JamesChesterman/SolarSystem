#ifndef SATELLITE_H
#define SATELLITE_H

#include "Sphere.h"

class Satellite : public Sphere {
private:
	float radius, angularSpeed, currentAngle;
	Vector3 centrePos;
public:
	//Inherits constructors from Sphere class
	using Sphere::Sphere;
	void setOrbitParams(Vector3 cPos, float r, float as);
	void updateOrbit(float deltaTime);
	void setCentrePos(Vector3 cPos);
};

#endif
