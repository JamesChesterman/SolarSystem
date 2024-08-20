#ifndef SPHERE_H
#define SPHERE_H

#include<vector>

struct Color {
	float r, g, b;
};

struct Vector3 {
	float x, y, z;
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
	float massKg;
	Color color;
protected:
	Vector3 pos;
public:
	//Not used initialiser list here so that other things can happen in the constructor
	Sphere(float x = 0, float y = 0, float z = 0, float massKg = 0);
	void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int latDivisions, unsigned int longDivisions, float radius);
	void setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void setupUniforms(bool isSun = false);
	void translate(float dx, float dy, float dz, float deltaTime);
	//Getter methods are here to improve performance
	unsigned int getShaderProgram() const { return shaderProgram; }
	unsigned int getVAO() const { return VAO; }
	Vector3 getPos() const { return pos; }
	//Setter methods:
	void setColor(float r, float g, float b);
};


#endif


