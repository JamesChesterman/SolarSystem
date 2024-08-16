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

Camera::Camera() {
	pos = { 0, 0, 0 };
}

void Camera::setCameraPos(Vector3 newPos, Sphere sphere) {
	pos = newPos;
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(pos.x, pos.y, pos.z),    //Camera pos
        glm::vec3(0.0f, 0.0f, 0.0f),    //Look at pos
        glm::vec3(0.0f, 0.0f, -1.0f));  //Up direction
        
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Window::getWindowWidth() / (float)Window::getWindowHeight(), 0.1f, 2000.0f);

    unsigned int shaderProgram = sphere.getShaderProgram();
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(viewPosLoc, pos.x, pos.y, pos.z);   // Camera/view position
}


