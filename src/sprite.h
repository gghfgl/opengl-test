#pragma once

#include <glm/glm.hpp>

#include "texture.h"

struct Sprite {
    glm::vec2 position, size, velocity;
    glm::vec3 color;
    float32 rotation;

    Texture2D texture;	
};

Sprite* InitSprite(Texture2D texture, glm::vec2 pos, glm::vec2 size, glm::vec3 color) {
    Sprite* sprite = new Sprite;
    if (!sprite) {
        Log::error("RENDERER: error wile allocating Sprite memory");
    }

    sprite->position = pos;
    sprite->size = size;
    sprite->texture = texture;
    sprite->color = color;
    sprite->velocity = glm::vec2(0.0f);
    sprite->rotation = 0.0f;

    return sprite;
}
