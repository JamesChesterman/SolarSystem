#include<glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"

Renderer::Renderer() {
    vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
    vertexShader = 0;
    fragmentShader = 0;
    shaderProgram = 0;
    VAO = 0;
    success = 0;

    //VAO needs to be made before VBO
    makeVAO();

    makeVBO();

    makeVertexShader();
    makeFragmentShader();
    makeShaderProgram();

    makeLinker();
}

void Renderer::makeVAO() {
    //OpenGL requires a VAO. 
    //Remembers which data goes where. So can easily switch between sets of data for drawing shapes.
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

}

void Renderer::makeVBO() {
    //Vertex coordinates should be in normalised device coordinates
    //This means the x,y,z values should range from -1.0 to 1.0. 
    //Any coords outside this range will be clipped from view.
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    //VBO = Vertex Buffer Object. Can store many vertices in GPU memory
    //Should send data to GPU in large batches as sending it is slow.
    unsigned int VBO;
    //& used here to send the address of the VBO var
    glGenBuffers(1, &VBO);
    //Bind the VBO to the type 'GL_ARRAY_BUFFER' as this is used for a vertex buffer object
    //From now on any call made to the GL_ARRAY_BUFFER will be used to configure the VBO object (as this command binds it)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copies data to the currently bound buffer
    //sizeof(vertices) passes the size of the data being passed into it. vertices is the data itself

    //GL_STREAM_DRAW = data set once. Used by the GPU at most a few times
    //GL_STATIC_DRAW = data set once. Used many times
    //GL_DYNAMIC_DRAW = data changed a lot. Used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Vertex data stored in graphics card memory. Now want to create vertex + fragment shader to process it
}

//Don't need an & here. Int is efficient enough to return by value.
void Renderer::makeVertexShader() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach shader source code (in vertexShaderSource) to the shader obj
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Checks if there are any errors when compiling shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Renderer::makeFragmentShader() {
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Renderer::makeShaderProgram() {
    //Make a Shader Program object
    //Then activate this shader program when rendering objs.
    //Will link the outputs of each shade to the errors of the next shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Results in a program obj we can activate by calling glUseProgram with the program obj as its argument
    //glUseProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Delete shader objects once they've been linked.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::makeLinker() {
    //Need to specify how OpenGL interprets vertex data before rendering.
    //Each position has 3 values. Each value is 4 bytes. So it's 12 bytes in total.
    //Second arg means there are 3 vals in vertex. Third arg means they're float values.
    //5th arg means the amount of stride between each vertex attribute. There are 3 floats for each vertex.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}



