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

    uint32 internalFormat = PLATEFORM::RGB;
    uint32 imageFormat = PLATEFORM::RGB;
    uint32 wrapAxisS = PLATEFORM::REPEAT;
    uint32 wrapAxisT = PLATEFORM::REPEAT;
    uint32 filterMin = PLATEFORM::LINEAR;
    uint32 filterMax = PLATEFORM::LINEAR;

    if (alpha) {
        internalFormat = PLATEFORM::RGBA;
        imageFormat = PLATEFORM::RGBA;
    }

    Texture2D texture;
    texture.ID = PLATEFORM::GenerateTexture(width, height, internalFormat, imageFormat, wrapAxisS, wrapAxisT, filterMin, filterMax, data);
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
        PLATEFORM::DeleteTexture(&iter.second.ID);
}
