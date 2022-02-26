#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "sprite.h"


struct SpriteRenderer {
    uint32 VAO;
};

void init_sprite_buffer(SpriteRenderer* renderer);

SpriteRenderer* InitSpriteRenderer() {
    SpriteRenderer* renderer = new SpriteRenderer;
    renderer->VAO = 0;

    init_sprite_buffer(renderer);

    return renderer;
}

// @unused: Should be used right?
void ClearSpriteRenderer(SpriteRenderer renderer) {
    glDeleteVertexArrays(1, &renderer.VAO);    
}

void DrawSprite(SpriteRenderer* renderer, Sprite* sprite, Shader shader) {
    UseShader(shader);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite->position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * sprite->size.x, 0.5f * sprite->size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(sprite->rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * sprite->size.x, -0.5f * sprite->size.y, 0.0f)); // move origin back
    model = glm::scale(model, glm::vec3(sprite->size, 1.0f)); // last scale

    SetShaderMatrix4(shader, "model", model);
    SetShaderVector3f(shader, "spriteColor", sprite->color);

    glActiveTexture(GL_TEXTURE0);
    BindTexture(&sprite->texture);

    glBindVertexArray(renderer->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void init_sprite_buffer(SpriteRenderer* renderer) {
    uint32 VBO;
    float32 vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &renderer->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(renderer->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float32), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
