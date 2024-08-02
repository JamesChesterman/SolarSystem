#ifndef SPHERE_H
#define SPHERE_H

#include<vector>

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
public:
	Sphere();
	void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int latDivisions, unsigned int longDivisions, float radius);
	void setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void setupUniforms();
	//Getter methods are here to improve performance
	unsigned int getShaderProgram() const { return shaderProgram; }
	unsigned int getVAO() const { return VAO; }
};


#endif


