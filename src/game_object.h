#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

struct GameObject {
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    float32 Rotation;
    bool IsSolid;
    bool Destroyed;

    Texture2D Sprite;	
};

GameObject InitGameObject(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color) {
    GameObject game;
    game.Position = pos;
    game.Size = size;
    game.Sprite = sprite;
    game.Color = color;
    game.Velocity = glm::vec2(0.0f);
    game.Rotation = 0.0f;
    game.IsSolid = false;
    game.Destroyed = false;

    return game;
}
