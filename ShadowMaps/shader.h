#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <QFile>
#include <QTextStream>
#include "glad.h"

class Shader
{
public:
    unsigned int ID;
    Shader(){}
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        Compile(vertexPath, fragmentPath);
    }

    void Compile(const char* vertexPath, const char* fragmentPath)
    {
        QFile vertFile(vertexPath);
        vertFile.open(QFile::ReadOnly | QFile::Text);
        QString vertString;
        QTextStream vertStream(&vertFile);
        vertString.append(vertStream.readAll());
        std::string vertSTLString = vertString.toStdString();

        // read fragment shader from Qt resource file
        QFile fragFile(fragmentPath);
        fragFile.open(QFile::ReadOnly | QFile::Text);
        QString fragString;
        QTextStream fragStream(&fragFile);
        fragString.append(fragStream.readAll());
        std::string fragSTLString = fragString.toStdString();

        const char* vShaderCode = vertSTLString.c_str();//vertexCode.c_str();
        const char* fShaderCode = fragSTLString.c_str();//fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    Shader &use()
    {
        glUseProgram(ID);
        return *this;
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, glm::vec2 v) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()),1,&v[0]);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, float x,float y,float z) const
    {
        glm::vec3 v(x,y,z);
        glUniform3fv(glGetUniformLocation(ID, name.c_str()),1,&v[0]);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, glm::vec3 v) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()),1,&v[0]);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, glm::vec4 v) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()),1,&v[0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name,  glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }


private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else
            {
                std::cout << "SAHDER COMPILE SUCCESS: " << shader << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else
            {
                std::cout << "SAHDER LINK SUCCESS " << std::endl;
            }
        }
    }
};

#endif // SHADER_H
