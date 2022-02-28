#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

enum ShaderType {
    NONE     = -1,
    VERTEX   =  0,
    FRAGMENT =  1,
    // GEOMETRY =  2
};

struct Shader {
    uint32 ID;
};

void AddShaderToCache(Shader shader, std::string key, std::map<std::string, Shader> &cache) {
    cache[key] = shader;
}

Shader GetShaderFromCache(std::map<std::string, Shader> &cache, std::string key) {
    return cache[key];
}

void ClearShaderCache(std::map<std::string, Shader> &cache) {
    for (auto iter : cache)
        DeleteShader(iter.second.ID);
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
    shader.ID = CompileShaderSources(ss[(int)ShaderType::VERTEX].str().c_str(), ss[(int)ShaderType::FRAGMENT].str().c_str());

    Log::info("SHADER: compile success [%s]\n", filepath);

    return shader;
}
