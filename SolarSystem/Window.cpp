#include<glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
    Sphere sphere;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    sphere.generateSphere(vertices, indices, 20, 20, 1.0f);
    sphere.setupBuffers(vertices, indices);

    glUseProgram(sphere.getShaderProgram());
        
    setupUniforms(sphere);

    glEnable(GL_DEPTH_TEST);

    //Rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //Rendering commands go here

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(sphere.getVAO());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        //Double buffering used to load next series of pixels whilst drawing current pixels
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::exit(0);
}

void Window::setupUniforms(Sphere& sphere) {
    //Set up uniform values for model, view, projection matrices
    //Also lighting parameters.
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);
    
    //These are all attributes in the glsl shader source (in Sphere.cpp)
    unsigned int modelLoc = glGetUniformLocation(sphere.getShaderProgram(), "model");
    unsigned int viewLoc = glGetUniformLocation(sphere.getShaderProgram(), "view");
    unsigned int projLoc = glGetUniformLocation(sphere.getShaderProgram(), "projection");
    unsigned int colorLoc = glGetUniformLocation(sphere.getShaderProgram(), "objectColor");
    unsigned int lightPosLoc = glGetUniformLocation(sphere.getShaderProgram(), "lightPos");
    unsigned int lightColorLoc = glGetUniformLocation(sphere.getShaderProgram(), "lightColor");
    unsigned int viewPosLoc = glGetUniformLocation(sphere.getShaderProgram(), "viewPos");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);

    glUniform3f(lightPosLoc, 1.0f, 1.0f, 1.0f);  // Position of the light source
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 5.0f);   // Camera/view position
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // White light
}



int Window::getWindowWidth() const {
    return Window::WINDOWWIDTH;
}

int Window::getWindowHeight() const {
    return Window::WINDOWHEIGHT;
}

