#ifndef SPHERE_H
#define SPHERE_H

#include<vector>

struct Color {
	float r, g, b;

	//Constructor to avoid any errors
	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f) : r(r), g(g), b(b) {};
};

class Sphere {
private:
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VAO, VBO, EBO;
	void makeShaderProgram();
	unsigned int compileShader(unsigned int type, const char* source);
	float x, y, z, massKg;
	Color color;
public:
	//Not used initialiser list here so that other things can happen in the constructor
	Sphere(float x = 0, float y = 0, float z = 0, float massKg = 0);
	void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int latDivisions, unsigned int longDivisions, float radius);
	void setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void setupUniforms();
	void translate(float dx, float dy, float dz, float deltaTime);
	//Getter methods are here to improve performance
	unsigned int getShaderProgram() const { return shaderProgram; }
	unsigned int getVAO() const { return VAO; }
	//Setter methods:
	void setColor(float r, float g, float b);
};


#endif


