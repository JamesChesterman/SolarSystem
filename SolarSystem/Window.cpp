#include<glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//The include "Window.h" must be below the other includes
#include "Window.h"
#include "Sphere.h"
#include "Satellite.h"



void Window::initGLFW() {
    //Initialise GLFW library
    glfwInit();
    //Initialise OpenGL settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Window::createWindow() {
    //Window creation
    //glfwCreateWindow returns a pointer to the memory
    //Not returning a pointer would maybe duplicate the memory required multiple times
    window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Solar System", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool Window::initGLAD() {
    //GLAD manages function pointers for OpenGL so init GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return false;
    }
    return true;
}

Window::Window() {
    initGLFW();
    createWindow();
    initGLAD();

    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
    
    render();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //Will close the window on the next iteration of the rendering loop
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::render() {
    //Setup bodies:
    Sphere sun(0, 0, 0, 1);
    sun.setColor(1.0f, 0.65f, 0.0f);     //Orange
    std::vector<float> verticesSun;
    std::vector<unsigned int> indicesSun;
    //Sun's radius is 109x that of earth
    sun.generateSphere(verticesSun, indicesSun, 20, 20, 10.9f);
    sun.setupBuffers(verticesSun, indicesSun);

    Satellite earth(0, 0, -75, 1);
    earth.setColor(0, 0, 0.9f);
    earth.setOrbitParams(0.0f, 0.0f, 0.0f, 75.0f, 1.0f);
    std::vector<float> verticesEarth;
    std::vector<unsigned int> indicesEarth;
    earth.generateSphere(verticesEarth, indicesEarth, 20, 20, 1.0f);
    earth.setupBuffers(verticesEarth, indicesEarth);

    Satellite moon(0, 0, -85, 1);
    moon.setColor(0, 0, 0);
    moon.setOrbitParams(earth.getX(), earth.getY(), earth.getZ(), 10, 2);
    std::vector<float> verticesMoon;
    std::vector<unsigned int> indicesMoon;
    moon.generateSphere(verticesMoon, indicesMoon, 20, 20, 0.5f);
    moon.setupBuffers(verticesMoon, indicesMoon);


    glUseProgram(sun.getShaderProgram());

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0;
    float lastFrame = 0;

    //Rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Bodies in Motion:
        earth.updateOrbit(deltaTime);
        moon.setCentrePos(earth.getX(), earth.getY(), earth.getZ());
        moon.updateOrbit(deltaTime);

        //Rendering commands go here

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //All bodies:
        sun.setupUniforms();
        glBindVertexArray(sun.getVAO());
        glDrawElements(GL_TRIANGLES, indicesSun.size(), GL_UNSIGNED_INT, 0);

        earth.setupUniforms();
        glBindVertexArray(earth.getVAO());
        glDrawElements(GL_TRIANGLES, indicesEarth.size(), GL_UNSIGNED_INT, 0);

        moon.setupUniforms();
        glBindVertexArray(moon.getVAO());
        glDrawElements(GL_TRIANGLES, indicesMoon.size(), GL_UNSIGNED_INT, 0);

        //Double buffering used to load next series of pixels whilst drawing current pixels
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::exit(0);
}



int Window::getWindowWidth(){
    return WINDOWWIDTH;
}

int Window::getWindowHeight(){
    return WINDOWHEIGHT;
}

