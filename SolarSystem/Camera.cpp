#include<glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Camera.h"
#include "Window.h"
#include "Sphere.h"

#ifndef M+PI
#define M_PI 3.14159265358979323846
#endif


Camera::Camera() {
    //TODO Change this. Then have max and min values for angles
    posSphere = { 1000,M_PI / 4,M_PI / 2};
    pos = translatePos(posSphere);
}

void Camera::update(Sphere& sphere) {
    glm::mat4 view = glm::lookAt(
        glm::vec3(pos.x, pos.y, pos.z),    //Camera pos
        glm::vec3(0.0f, 0.0f, 0.0f),    //Look at pos
        glm::vec3(0.0f, 1.0f, 0.0f));  //Up direction

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Window::getWindowWidth() / (float)Window::getWindowHeight(), 0.1f, 5000.0f);

    unsigned int shaderProgram = sphere.getShaderProgram();
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(viewPosLoc, pos.x, pos.y, pos.z);   // Camera/view position
}

//Angles are in radians
void Camera::move(std::string direction) {
    
    if (direction == "up") {
        posSphere.polarAngle += angleSpeed;
    }
    else if (direction == "down") {
        posSphere.polarAngle -= angleSpeed;
    }
    else if (direction == "forward") {
        posSphere.radius -= speed;
    }
    else if (direction == "backward") {
        posSphere.radius += speed;
    }
    pos = translatePos(posSphere);
}

Vector3 Camera::translatePos(sphereCoords coords) {
    Vector3 newPos;
    //Angles are in radians
    newPos.x = coords.radius * sin(coords.polarAngle) * cos(coords.azimuthAngle);
    newPos.y = coords.radius * sin(coords.polarAngle) * sin(coords.azimuthAngle);
    newPos.z = coords.radius * cos(coords.polarAngle);
    return newPos;
}


