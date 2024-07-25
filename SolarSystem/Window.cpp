#include<glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//The include "Window.h" must be below the other includes

#include "Window.h"

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
    Window::initGLFW();
    Window::createWindow();
    Window::initGLAD();

    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);

    //Rendering loop
    while (!glfwWindowShouldClose(window)) {
        //Double buffering used to load next series of pixels whilst drawing current pixels
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::exit(0);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int Window::getWindowWidth() const {
    return Window::WINDOWWIDTH;
}

int Window::getWindowHeight() const {
    return Window::WINDOWHEIGHT;
}

