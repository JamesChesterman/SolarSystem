#include<glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//The include "Window.h" must be below the other includes
#include "Window.h"
#include "Sphere.h"



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
    Sphere sphere(0, 0, -2, 1);
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    sphere.generateSphere(vertices, indices, 20, 20, 0.5f);
    sphere.setupBuffers(vertices, indices);

    Sphere sphere2(0, 0, -2, 1);
    std::vector<float> vertices2;
    std::vector<unsigned int> indices2;
    sphere2.generateSphere(vertices2, indices2, 20, 20, 0.5f);
    sphere2.setupBuffers(vertices2, indices2);

    glUseProgram(sphere.getShaderProgram());

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0;
    float lastFrame = 0;

    //Rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sphere.translate(0.1f, 0.1f, 0.1f, deltaTime);
        sphere2.translate(-0.1f, -0.1f, 0.1f, deltaTime);

        //Rendering commands go here

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sphere.setupUniforms();
        glBindVertexArray(sphere.getVAO());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        sphere2.setupUniforms();
        glBindVertexArray(sphere2.getVAO());
        glDrawElements(GL_TRIANGLES, indices2.size(), GL_UNSIGNED_INT, 0);

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

