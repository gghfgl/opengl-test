#pragma once

#include <map>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture2D {
    uint32 ID;
};

Texture2D LoadTextureFromFile(const char *file, bool alpha) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (!data) {
        Log::error("TEXTURE: Failed to load image from file: %s\n", file);
    }

    uint32 internalFormat = PLATEFORM_RGB;
    uint32 imageFormat = PLATEFORM_RGB;
    uint32 wrapAxisS = PLATEFORM_REPEAT;
    uint32 wrapAxisT = PLATEFORM_REPEAT;
    uint32 filterMin = PLATEFORM_LINEAR;
    uint32 filterMax = PLATEFORM_LINEAR;

    if (alpha) {
        internalFormat = PLATEFORM_RGBA;
        imageFormat = PLATEFORM_RGBA;
    }

    Texture2D texture;
    texture.ID = GenerateTexture(width, height, internalFormat, imageFormat, wrapAxisS, wrapAxisT, filterMin, filterMax, data);
    stbi_image_free(data);

    return texture;
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
        DeleteTexture(&iter.second.ID);
}
