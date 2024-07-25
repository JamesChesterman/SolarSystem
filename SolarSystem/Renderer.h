#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
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
	Renderer();
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
