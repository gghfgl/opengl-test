#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ShaderType {
    NONE     = -1,
    VERTEX   =  0,
    FRAGMENT =  1,
    // GEOMETRY =  2
};

struct Shader {
    uint32 ID;
};

void compileShaderSources(Shader* shader, const char* vertexSource, const char* fragmentSource);
void checkShaderCompileErrors(uint32 object, std::string type);

void SetShaderFloat(Shader shader, const char *name, float32 value) {
    glUniform1f(glGetUniformLocation(shader.ID, name), value);
}

void SetShaderInteger(Shader shader, const char *name, int32 value) {
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void SetShaderVector2f(Shader shader, const char *name, float32 x, float32 y) {
    glUniform2f(glGetUniformLocation(shader.ID, name), x, y);
}

void SetShaderVector2f(Shader shader, const char *name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(shader.ID, name), value.x, value.y);
}

void SetShaderVector3f(Shader shader, const char *name, float32 x, float32 y, float32 z) {
    glUniform3f(glGetUniformLocation(shader.ID, name), x, y, z);
}

void SetShaderVector3f(Shader shader, const char *name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(shader.ID, name), value.x, value.y, value.z);
}

void SetShaderVector4f(Shader shader, const char *name, float32 x, float32 y, float32 z, float32 w) {
    glUniform4f(glGetUniformLocation(shader.ID, name), x, y, z, w);
}

void SetShaderVector4f(Shader shader, const char *name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(shader.ID, name), value.x, value.y, value.z, value.w);
}

void SetShaderMatrix4(Shader shader, const char *name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, false, glm::value_ptr(matrix));
}

void UseShader(Shader shader) {
    glUseProgram(shader.ID);
}

void AddShaderToCache(Shader shader, std::string key, std::map<std::string, Shader> &cache) {
    cache[key] = shader;
}

Shader GetShaderFromCache(std::map<std::string, Shader> &cache, std::string key) {
    return cache[key];
}

void ClearShaderCache(std::map<std::string, Shader> &cache) {
    for (auto iter : cache)
        glDeleteProgram(iter.second.ID);
}

Shader LoadShaderFromFile(const char *filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    Shader shader;
    compileShaderSources(&shader, ss[(int)ShaderType::VERTEX].str().c_str(), ss[(int)ShaderType::FRAGMENT].str().c_str());

    Log::info("SHADER: success compile shader [%s]\n", filepath);

    return shader;
}

void compileShaderSources(Shader* shader, const char* vertexSource, const char* fragmentSource) {
    uint32 vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertexShader);
    glAttachShader(shader->ID, fragmentShader);
    glLinkProgram(shader->ID);

    checkShaderCompileErrors(shader->ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void checkShaderCompileErrors(uint32 object, std::string type) {
    int32 success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            Log::error("SHADER: Compile-time error: Type: %s\n %s\n", type.c_str(), infoLog);
        }
    } else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            Log::error("SHADER: Link-time error: Type: %s\n %s\n", type.c_str(), infoLog);
        }
    }
}
