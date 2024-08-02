#ifndef WINDOW_H
#define WINDOW_H

#include<GLFW/glfw3.h>
#include "Sphere.h"


class Window {
private:
	GLFWwindow* window;
	static const int WINDOWWIDTH = 800;
	static const int WINDOWHEIGHT = 600;
	void initGLFW();
	bool createWindow();
	bool initGLAD();

public:
	Window();
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void render();
	static int getWindowWidth();
	static int getWindowHeight();
};

#endif
