#include<glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Sphere.h"

#ifndef M+PI
#define M_PI 3.14159265358979323846
#endif

void Sphere::setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}

Sphere::Sphere(float x, float y, float z, float massKg) {
    //Shader source code is AI Generated.
    //Modified to include whether or not the object is the Sun.
    vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;

        out vec3 FragPos;
        out vec3 Normal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

    fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 lightColor;
        uniform vec3 objectColor;
        uniform bool isSun;

        void main() {
            if(isSun){
                FragColor = vec4(objectColor, 1.0);
            }else{
                float ambientStrength = 0.3;
                vec3 ambient = ambientStrength * lightColor;

                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;

                float specularStrength = 0.8;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColor;

                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            }
            
        }
    )";
    pos = { x, y, z };
    this->massKg = massKg;

    makeShaderProgram();
}


//A lot of this code is taken from AI Generation.
void Sphere::generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int latDivisions, unsigned int longDivisions, float radius) {
    vertices.clear();
    indices.clear();

    for (int lat = 0; lat <= latDivisions; ++lat) {
        float theta = lat * M_PI / latDivisions;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longDivisions; ++lon) {
            float phi = lon * 2.0f * M_PI / longDivisions;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            //Vertex position being pushed
            vertices.push_back(radius * x);
            vertices.push_back(radius * y);
            vertices.push_back(radius * z);

            //Normal
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int lat = 0; lat < latDivisions; ++lat) {
        for (int lon = 0; lon < longDivisions; ++lon) {
            int first = (lat * (latDivisions + 1)) + lon;
            int second = first + longDivisions + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}


unsigned int Sphere::compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

void Sphere::makeShaderProgram() {
    vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Sphere::setupUniforms(bool isSun) {
    //Set up uniform values for model, view, projection matrices
    //Also lighting parameters.
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));

    
    
    //These are all attributes in the glsl shader source (in Sphere.cpp)
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    
    
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    unsigned int isSunLoc = glGetUniformLocation(shaderProgram, "isSun");


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glUniform3f(colorLoc, color.r, color.g, color.b);
    glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);  // Position of the light source
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Make light white
    glUniform1i(isSunLoc, isSun);
}

//Keeping this method in case I might use it in the future.
void Sphere::translate(float dx, float dy, float dz, float deltaTime) {
    pos.x += (dx * deltaTime);
    pos.y += (dy * deltaTime);
    pos.z += (dz * deltaTime);
    setupUniforms();
}


void Sphere::setColor(float r, float g, float b) {
    color = { r, g, b };
}






