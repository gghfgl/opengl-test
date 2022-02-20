#pragma once

#include <map>
#include <string>
#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture2D {
    uint32 ID;
    uint32 Width, Height; // width and height of loaded image in pixels
    uint32 InternalFormat; // format of texture object
    uint32 ImageFormat; // format of loaded image
    uint32 WrapAxisS; // wrapping mode on S axis
    uint32 WrapAxisT; // wrapping mode on T axis
    uint32 FilterMin; // filtering mode if texture pixels < screen pixels
    uint32 FilterMax; // filtering mode if texture pixels > screen pixels
};

void generateTexture(Texture2D* texture, uint32 width, uint32 height, unsigned char* data);
Texture2D loadTextureFromFile(const char *file, bool alpha);

Texture2D InitTexture(uint32 width, uint32 height) {
    Texture2D texture;
    texture.Width = width;
    texture.Height = height;
    texture.InternalFormat = GL_RGB;
    texture.ImageFormat = GL_RGB;
    texture.WrapAxisS = GL_REPEAT;
    texture.WrapAxisT = GL_REPEAT;
    texture.FilterMin = GL_LINEAR;
    texture.FilterMax = GL_LINEAR;

    glGenTextures(1, &texture.ID);

    return texture;
}

void BindTexture(Texture2D* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}

Texture2D AddTextureToCache(std::map<std::string, Texture2D> cache, std::string name, const char *file, bool alpha) {
    cache[name] = loadTextureFromFile(file, alpha);
    return cache[name];
}

// @remove: Seems useless code.
Texture2D GetTextureFromCache(std::map<std::string, Texture2D> cache, std::string name) {
    return cache[name];
}

void ClearTextureCache(std::map<std::string, Texture2D> cache) {
    for (auto iter : cache)
        glDeleteTextures(1, &iter.second.ID);
}

// @improve: Use InitTexture function.
Texture2D loadTextureFromFile(const char *file, bool alpha) {
    Texture2D texture;
    if (alpha) {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    generateTexture(&texture, width, height, data);

    stbi_image_free(data);

    return texture;
}

void generateTexture(Texture2D* texture, uint32 width, uint32 height, unsigned char* data) {
    texture->Width = width;
    texture->Height = height;

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->InternalFormat, width, height, 0, texture->ImageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->WrapAxisS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->WrapAxisT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->FilterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}
