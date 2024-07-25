#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle {
private:
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VAO;
	int success;
	char infoLog[512];

public:
	Triangle();
	void drawTriangle();
	void makeVAO();
	void makeVBO();
	void makeVertexShader();
	void makeFragmentShader();
	void makeShaderProgram();
	void makeLinker();
	//Getter methods are here to improve performance
	unsigned int getShaderProgram() const { return shaderProgram; }
	unsigned int getVAO() const { return VAO; }
};

#endif
