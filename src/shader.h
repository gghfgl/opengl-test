#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shader
{
    unsigned int ID; 

    Shader() { }

    Shader &Use() {
        glUseProgram(this->ID);
        return *this;
    }

    void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr) {
        unsigned int sVertex, sFragment, gShader;

        // vertex Shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        checkCompileErrors(sVertex, "VERTEX");

        // fragment Shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        checkCompileErrors(sFragment, "FRAGMENT");

        // if geometry shader source code is given, also compile geometry shader
        if (geometrySource != nullptr)
        {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, NULL);
            glCompileShader(gShader);
            checkCompileErrors(gShader, "GEOMETRY");
        }

        // shader program
        this->ID = glCreateProgram();
        glAttachShader(this->ID, sVertex);
        glAttachShader(this->ID, sFragment);
        if (geometrySource != nullptr)
            glAttachShader(this->ID, gShader);
        glLinkProgram(this->ID);
        checkCompileErrors(this->ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr)
            glDeleteShader(gShader);        
    }

    void SetFloat(const char *name, float value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform1f(glGetUniformLocation(this->ID, name), value);
    }

    void SetInteger(const char *name, int value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform1i(glGetUniformLocation(this->ID, name), value);
    }

    void SetVector2f(const char *name, float x, float y, bool useShader) {
        if (useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), x, y);
    }

    void SetVector2f(const char *name, const glm::vec2 &value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
    }

    void SetVector3f(const char *name, float x, float y, float z, bool useShader) {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
    }

    void SetVector3f(const char *name, const glm::vec3 &value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
    }

    void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader) {
        if (useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
    }

    void SetVector4f(const char *name, const glm::vec4 &value, bool useShader) {
        if (useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
    }

    void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader) {
        if (useShader)
            this->Use();
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
    }

    void checkCompileErrors(unsigned int object, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
    }
};

#endif
