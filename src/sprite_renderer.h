#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

// @improve: Rework this part properly.
struct SpriteRenderer {
    Shader shader;
    uint32 quadVAO;    
};

void initRendererData(SpriteRenderer* renderer);

SpriteRenderer* InitSpriteRenderer(Shader shader) {
    SpriteRenderer* renderer = new SpriteRenderer; // @improve: Migrate to a better memory management.
    renderer->shader = shader;
    initRendererData(renderer);

    return renderer;
}

void ClearSpriteRenderer(SpriteRenderer* renderer) {
    glDeleteVertexArrays(1, &renderer->quadVAO);    
}

void DrawSprite(SpriteRenderer* renderer, Texture2D &texture, glm::vec2 position, glm::vec2 size, float32 rotate, glm::vec3 color) {
    // prepare transformations
    UseShader(renderer->shader);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    SetShaderMatrix4(renderer->shader, "model", model);

    // render textured quad
    SetShaderVector3f(renderer->shader, "spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    BindTexture(&texture);

    glBindVertexArray(renderer->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void initRendererData(SpriteRenderer* renderer) {
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

    glGenVertexArrays(1, &renderer->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(renderer->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float32), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
