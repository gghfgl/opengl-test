#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"
#include "sprite.h"


struct SpriteRenderer {
    uint32 VAO;
    uint32 drawCalls; // @improve: Good idea?
};

uint32 bind_sprite_buffer();

SpriteRenderer InitSpriteRenderer() {
    SpriteRenderer renderer;
    renderer.VAO = bind_sprite_buffer();

    return renderer;
}

// @improve: Useless? Maybe call directly plateform API.
void ClearSpriteRenderer(SpriteRenderer* renderer) {
    PLATEFORM::DeleteVertexArrays(&renderer->VAO);    
}

void DrawSprite(SpriteRenderer* renderer, Sprite* sprite, Shader shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite->position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * sprite->size.x, 0.5f * sprite->size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(sprite->rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * sprite->size.x, -0.5f * sprite->size.y, 0.0f)); // move origin back
    model = glm::scale(model, glm::vec3(sprite->size, 1.0f)); // last scale

    PLATEFORM::UseShader(shader.ID);
    PLATEFORM::SetShaderMatrix4(shader.ID, "model", model);
    PLATEFORM::SetShaderVector3f(shader.ID, "spriteColor", sprite->color);

    PLATEFORM::BindTexture(sprite->texture.ID);
    PLATEFORM::DrawVertexArraysTriangles(renderer->VAO);
}

uint32 bind_sprite_buffer() {
    float32 vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    uint32 VAO = PLATEFORM::BindVertexArraysFloat(vertices, (uint32)sizeof(vertices), 4);

    return VAO;
}
