#ifndef WINDOW_H
#define WINDOW_H

#include<GLFW/glfw3.h>
#include "Sphere.h"


class Window {
private:
	GLFWwindow* window;
	const int WINDOWWIDTH = 800;
	const int WINDOWHEIGHT = 600;
	void initGLFW();
	bool createWindow();
	bool initGLAD();

public:
	Window();
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void render();
	void setupUniforms(Sphere& sphere);
	int getWindowWidth() const;
	int getWindowHeight() const;
};

#endif
