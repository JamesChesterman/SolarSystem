#include<glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//The include "Window.h" must be below the other includes
#include "Window.h"
#include "Sphere.h"
#include "Satellite.h"
#include "Camera.h"


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

void Window::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //Will close the window on the next iteration of the rendering loop
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move("up");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move("down");
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.move("forward");
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.move("backward");
    }

}

//Tried to make this stuff more efficient.
//Don't try to make Sphere sun, Satellite earth etc into member variables
//Because otherwise their constructors will be called (in the header files)
//So OpenGL will try to do the stuff in Sphere without having been initialised (here)
void Window::render() {
    int sunDiameter = 100;
    //Setup bodies:
    Sphere sun(0, 0, 0, 1);
    sun.setColor(1.0f, 0.65f, 0.0f);     //Orange
    std::vector<float> verticesSun;
    std::vector<unsigned int> indicesSun;
    //Sun's radius is 109x that of earth
    sun.generateSphere(verticesSun, indicesSun, 50, 50, sunDiameter);
    sun.setupBuffers(verticesSun, indicesSun);

    Satellite mercury(0, 0, -19.3 - sunDiameter, 1);
    mercury.setColor(0.72f, 0.73f, 0.74f);
    mercury.setOrbitParams(Vector3{ 0, 0, 0 }, 19.3 + sunDiameter, 4.15f);
    std::vector<float> verticesMercury;
    std::vector<unsigned int> indicesMercury;
    mercury.generateSphere(verticesMercury, indicesMercury, 20, 20, 1.0f);
    mercury.setupBuffers(verticesMercury, indicesMercury);

    Satellite venus(0, 0, -36.06 - sunDiameter, 1);
    venus.setColor(0.57f, 0.52f, 0.56f);
    venus.setOrbitParams(Vector3{ 0, 0, 0 }, 36.06 + sunDiameter, 1.62f);
    std::vector<float> verticesVenus;
    std::vector<unsigned int> indicesVenus;
    venus.generateSphere(verticesVenus, indicesVenus, 20, 20, 2.82f);
    venus.setupBuffers(verticesVenus, indicesVenus);

    Satellite earth(0, 0, -49.87 - sunDiameter, 1);
    earth.setColor(0, 0, 0.9f);
    earth.setOrbitParams(Vector3 {0, 0, 0}, 49.87 + sunDiameter, 1.0f);
    std::vector<float> verticesEarth;
    std::vector<unsigned int> indicesEarth;
    earth.generateSphere(verticesEarth, indicesEarth, 20, 20, 3.0f);
    earth.setupBuffers(verticesEarth, indicesEarth);

    Satellite moon(0, 0, -53.71 - sunDiameter, 1);
    moon.setColor(0.62f, 0.63f, 0.64f);
    moon.setOrbitParams(earth.getPos(), 3.84, 13);
    std::vector<float> verticesMoon;
    std::vector<unsigned int> indicesMoon;
    moon.generateSphere(verticesMoon, indicesMoon, 20, 20, 0.75f);
    moon.setupBuffers(verticesMoon, indicesMoon);

    Satellite mars(0, 0, -76 - sunDiameter, 1);
    mars.setColor(0.63f, 0.14f, 0.1f);
    mars.setOrbitParams(Vector3{ 0,0,0 }, 76 + sunDiameter, 0.53f);
    std::vector<float> verticesMars;
    std::vector<unsigned int> indicesMars;
    mars.generateSphere(verticesMars, indicesMars, 20, 20, 1.6f);
    mars.setupBuffers(verticesMars, indicesMars);

    Satellite jupiter(0, 0, -259 - sunDiameter, 1);
    jupiter.setColor(0.79f, 0.56f, 0.22f);
    jupiter.setOrbitParams(Vector3{ 0, 0, 0 }, 259 + sunDiameter, 0.084f);
    std::vector<float> verticesJupiter;
    std::vector<unsigned int> indicesJupiter;
    jupiter.generateSphere(verticesJupiter, indicesJupiter, 40, 40, 32.87f);
    jupiter.setupBuffers(verticesJupiter, indicesJupiter);

    Satellite saturn(0, 0, -475.6f - sunDiameter, 1);
    saturn.setColor(0.77f, 0.69f, 0.55f);
    saturn.setOrbitParams(Vector3{ 0, 0, 0 }, 475.6 + sunDiameter, 0.034f);
    std::vector<float> verticesSaturn;
    std::vector<unsigned int> indicesSaturn;
    saturn.generateSphere(verticesSaturn, indicesSaturn, 40, 40, 28.33f);
    saturn.setupBuffers(verticesSaturn, indicesSaturn);

    Satellite uranus(0, 0, -957 - sunDiameter, 1);
    uranus.setColor(0.82f, 0.9f, 0.9f);
    uranus.setOrbitParams(Vector3{ 0,0,0 }, 957 + sunDiameter, 0.012f);
    std::vector<float> verticesUranus;
    std::vector<unsigned int> indicesUranus;
    uranus.generateSphere(verticesUranus, indicesUranus, 20, 20, 7.47f);
    uranus.setupBuffers(verticesUranus, indicesUranus);

    Satellite neptune(0, 0, -1499 - sunDiameter, 1);
    neptune.setColor(0.15f, 0.27f, 0.53f);
    neptune.setOrbitParams(Vector3{ 0,0,0 }, 1499 + sunDiameter, 0.006f);
    std::vector<float> verticesNeptune;
    std::vector<unsigned int> indicesNeptune;
    neptune.generateSphere(verticesNeptune, indicesNeptune, 20, 20, 11.6f);
    neptune.setupBuffers(verticesNeptune, indicesNeptune);



    glUseProgram(sun.getShaderProgram());

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0;
    float lastFrame = 0;

    //Rendering loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);

        //Bodies in Motion:
        mercury.updateOrbit(deltaTime);
        venus.updateOrbit(deltaTime);
        earth.updateOrbit(deltaTime);
        moon.setCentrePos(earth.getPos());
        moon.updateOrbit(deltaTime);
        mars.updateOrbit(deltaTime);
        jupiter.updateOrbit(deltaTime);
        saturn.updateOrbit(deltaTime);
        uranus.updateOrbit(deltaTime);
        neptune.updateOrbit(deltaTime);

        //Rendering commands go here

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update(sun);

        //All bodies:
        sun.setupUniforms(true);
        glBindVertexArray(sun.getVAO());
        glDrawElements(GL_TRIANGLES, indicesSun.size(), GL_UNSIGNED_INT, 0);

        mercury.setupUniforms();
        glBindVertexArray(mercury.getVAO());
        glDrawElements(GL_TRIANGLES, indicesMercury.size(), GL_UNSIGNED_INT, 0);
        
        venus.setupUniforms();
        glBindVertexArray(venus.getVAO());
        glDrawElements(GL_TRIANGLES, indicesVenus.size(), GL_UNSIGNED_INT, 0);
        
        earth.setupUniforms();
        glBindVertexArray(earth.getVAO());
        glDrawElements(GL_TRIANGLES, indicesEarth.size(), GL_UNSIGNED_INT, 0);

        moon.setupUniforms();
        glBindVertexArray(moon.getVAO());
        glDrawElements(GL_TRIANGLES, indicesMoon.size(), GL_UNSIGNED_INT, 0);

        mars.setupUniforms();
        glBindVertexArray(mars.getVAO());
        glDrawElements(GL_TRIANGLES, indicesMars.size(), GL_UNSIGNED_INT, 0);

        jupiter.setupUniforms();
        glBindVertexArray(jupiter.getVAO());
        glDrawElements(GL_TRIANGLES, indicesJupiter.size(), GL_UNSIGNED_INT, 0);

        saturn.setupUniforms();
        glBindVertexArray(saturn.getVAO());
        glDrawElements(GL_TRIANGLES, indicesSaturn.size(), GL_UNSIGNED_INT, 0);

        uranus.setupUniforms();
        glBindVertexArray(uranus.getVAO());
        glDrawElements(GL_TRIANGLES, indicesUranus.size(), GL_UNSIGNED_INT, 0);

        neptune.setupUniforms();
        glBindVertexArray(neptune.getVAO());
        glDrawElements(GL_TRIANGLES, indicesNeptune.size(), GL_UNSIGNED_INT, 0);

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

