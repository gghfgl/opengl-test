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

// @delete: Seems useless right?
Texture2D InitTexture(uint32 width, uint32 height, bool alpha) {
    Texture2D texture;
    texture.Width = width;
    texture.Height = height;
    texture.InternalFormat = GL_RGB;
    texture.ImageFormat = GL_RGB;
    texture.WrapAxisS = GL_REPEAT;
    texture.WrapAxisT = GL_REPEAT;
    texture.FilterMin = GL_LINEAR;
    texture.FilterMax = GL_LINEAR;

    if (alpha) {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

    glGenTextures(1, &texture.ID);

    return texture;
}

// @remove: Seems unused.
void BindTexture(Texture2D* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}

void AddTextureToCache(Texture2D texture, std::string key, std::map<std::string, Texture2D> &cache) {
    cache[key] = texture;
}

// @remove: Seems useless code.
Texture2D GetTextureFromCache(std::map<std::string, Texture2D> &cache, std::string key) {
    return cache[key];
}

void ClearTextureCache(std::map<std::string, Texture2D> &cache) {
    for (auto iter : cache)
        glDeleteTextures(1, &iter.second.ID);
}

void LoadTextureFromFile(Texture2D* texture, const char *file) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (!data) {
        Log::error("Failed to load image: %s\n", file);
    }

    generateTexture(texture, width, height, data);
    stbi_image_free(data);
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
