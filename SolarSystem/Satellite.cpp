#include <cmath>
#include "Sphere.h"
#include "Satellite.h"


void Satellite::setOrbitParams(Vector3 cPos, float r, float as) {
	centrePos = cPos;
	radius = r;
	//This is in radians per second
	angularSpeed = as;
	currentAngle = 0.0f;
}

void Satellite::updateOrbit(float deltaTime) {
	currentAngle += angularSpeed * deltaTime;

	//Need to change the z not the y because y is up and down.
	pos.x = centrePos.x + radius * cos(currentAngle);
	pos.z = centrePos.z + radius * sin(currentAngle);

	moveTo(pos.x, centrePos.y, pos.z);
}

void Satellite::setCentrePos(Vector3 cPos) {
	centrePos = cPos;
}