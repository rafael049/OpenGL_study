#ifndef SHADER_H
#define SHADER_H

#include "glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

using namespace glm;

class Shader{
public:
    unsigned int ID; // ID for shader program

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Compile Shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){

            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILDE\n" << infoLog << std::endl;
        }
        
        
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){

            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILDE\n" << infoLog << std::endl;
        }

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // debug
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        
        
        // delete shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        use();
    }

    void use(){
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const{
        glUseProgram(ID);
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const{ 
        glUseProgram(ID);
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const{ 
        glUseProgram(ID);
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setVec3(const std::string &name, vec3 value) const{ 
        glUseProgram(ID);
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value)); 
    }

    void setMat4f(const std::string &name, const GLfloat *value){
        glUseProgram(ID);
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value); 
    }
};




#endif
