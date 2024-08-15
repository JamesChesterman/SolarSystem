#include <cmath>
#include "Sphere.h"
#include "Satellite.h"


void Satellite::setOrbitParams(float cx, float cy, float cz, float r, float as) {
	centreX = cx;
	centreY = cy;
	centreZ = cz;
	radius = r;
	//This is in radians per second
	angularSpeed = as;
	currentAngle = 0.0f;
}

void Satellite::updateOrbit(float deltaTime) {
	currentAngle += angularSpeed * deltaTime;

	x = centreX + radius * cos(currentAngle);
	y = centreY + radius * sin(currentAngle);

	moveTo(x, y, centreZ);
}