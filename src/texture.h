#pragma once

#include <map>
#include <string>
#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture2D {
    uint32 ID;
    uint32 width, height; // width and height of loaded image in pixels
    uint32 internalFormat; // format of texture object
    uint32 imageFormat; // format of loaded image
    uint32 wrapAxisS; // wrapping mode on S axis
    uint32 wrapAxisT; // wrapping mode on T axis
    uint32 filterMin; // filtering mode if texture pixels < screen pixels
    uint32 filterMax; // filtering mode if texture pixels > screen pixels
};

void generateTexture(Texture2D* texture, uint32 width, uint32 height, unsigned char* data);

// @delete: Seems useless right?
Texture2D InitTexture(uint32 width, uint32 height, bool alpha) {
    Texture2D texture;
    texture.width = width;
    texture.height = height;
    texture.internalFormat = GL_RGB;
    texture.imageFormat = GL_RGB;
    texture.wrapAxisS = GL_REPEAT;
    texture.wrapAxisT = GL_REPEAT;
    texture.filterMin = GL_LINEAR;
    texture.filterMax = GL_LINEAR;

    if (alpha) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    glGenTextures(1, &texture.ID);

    return texture;
}

Texture2D LoadTextureFromFile(const char *file, bool alpha) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (!data) {
        Log::error("TEXTURE: Failed to load image from file: %s\n", file);
    }

    Texture2D texture;
    texture.width = width;
    texture.height = height;
    texture.internalFormat = GL_RGB;
    texture.imageFormat = GL_RGB;
    texture.wrapAxisS = GL_REPEAT;
    texture.wrapAxisT = GL_REPEAT;
    texture.filterMin = GL_LINEAR;
    texture.filterMax = GL_LINEAR;

    if (alpha) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    glGenTextures(1, &texture.ID);

    generateTexture(&texture, width, height, data);
    stbi_image_free(data);

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

void generateTexture(Texture2D* texture, uint32 width, uint32 height, unsigned char* data) {
    texture->width = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->internalFormat, width, height, 0, texture->imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapAxisS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapAxisT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}
