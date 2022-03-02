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

    // @improve: Meh, hidden behaviour
    renderer.VAO = bind_sprite_buffer();

    return renderer;
}

// @improve: Useless? Maybe call directly plateform API.
void ClearSpriteRenderer(SpriteRenderer* renderer) {
    DeleteVertexArrays(&renderer->VAO);    
}

void DrawSprite(SpriteRenderer* renderer, Sprite* sprite, Shader shader, glm::mat4 projection, glm::vec2 position, glm::mat4 zoomMatrix, float32 zoom) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite->position, 1.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * sprite->size.x, 0.5f * sprite->size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(sprite->rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * sprite->size.x, -0.5f * sprite->size.y, 0.0f)); // move origin back
    model = glm::scale(model, glm::vec3(sprite->size, 1.0f)); // last scale

    glm::mat4 lookAt = glm::lookAt(glm::vec3(position.x, position.y, 0.0f),
                                   glm::vec3(position.x, position.y, -1.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f));
    lookAt *= glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f)); // last scale

    UseShader(shader.ID);
    SetShaderMatrix4(shader.ID, "view", lookAt);
    SetShaderMatrix4(shader.ID, "projection", projection);
    SetShaderMatrix4(shader.ID, "model", model);
    SetShaderVector3f(shader.ID, "spriteColor", sprite->color);


    BindTexture(sprite->texture.ID);
    DrawVertexArraysTriangles(renderer->VAO);
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

    uint32 VAO = BindVertexArraysFloat(vertices, (uint32)sizeof(vertices), 4);

    return VAO;
}
