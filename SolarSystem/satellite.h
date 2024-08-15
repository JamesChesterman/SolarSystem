#ifndef SATELLITE_H
#define SATELLITE_H

#include "Sphere.h"

class Satellite : public Sphere {
private:
	float centreX, centreY, centreZ, radius, angularSpeed, currentAngle;

public:
	//Inherits constructors from Sphere class
	using Sphere::Sphere;
	void setOrbitParams(float cx, float cy, float cz, float r, float as);
	void updateOrbit(float deltaTime);
};

#endif
