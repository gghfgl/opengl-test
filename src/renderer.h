#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"


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

void DrawSprite(SpriteRenderer* renderer, Sprite* sprite, Shader shader, Camera* camera) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite->position, 1.0f));
    model = glm::scale(model, glm::vec3(sprite->size, 1.0f));

    // Prepare ortho projection with zoom and camera position offsets
    float32 xPos = 0.5f * camera->width + camera->position.x;
    float32 yPos = 0.5f * camera->height + camera->position.y;
    float32 left = -camera->width/(2*camera->zoom) + xPos;
    float32 right = camera->width/(2*camera->zoom) + xPos;
    float32 top = -camera->height/(2*camera->zoom) + yPos;
    float32 bottom = camera->height/(2*camera->zoom) + yPos;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

    UseShader(shader.ID);
    SetShaderMatrix4(shader.ID, "projection", projection);
    SetShaderMatrix4(shader.ID, "model", model);
    SetShaderVector3f(shader.ID, "color", sprite->color);

    BindTexture(sprite->texture.ID);
    DrawVertexArraysTriangles(renderer->VAO, 6); // 6 = the layout of sprite vertices array
}

/* void DrawLine(SpriteRenderer * renderer, Shader shader, Camera* camera) { */
/*     float32 xPos = 0.5f * camera->width + camera->position.x; */
/*     float32 yPos = 0.5f * camera->height + camera->position.y; */
/*     float32 left = -camera->width/(2*camera->zoom) + xPos; */
/*     float32 right = camera->width/(2*camera->zoom) + xPos; */
/*     float32 top = -camera->height/(2*camera->zoom) + yPos; */
/*     float32 bottom = camera->height/(2*camera->zoom) + yPos; */

/*     glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); */
/*     glm::mat4 model = glm::mat4(1.0f); */

/*     Shader lineShader = GetShaderFromCache(SHADER_CACHE, "line"); */
/*     UseShader(lineShader.ID); */
/*     SetShaderVector4f(lineShader.ID, "color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); */
/*     /\* SetShaderMatrix4(lineShader.ID, "projection", projection); *\/ */
/*     /\* SetShaderMatrix4(lineShader.ID, "model", model); *\/ */

/*     glLineWidth(3.0f); */
/*     glBindVertexArray(game->gridVAO); */
/*     glDrawArrays(GL_LINES, 0, 40); // maybe 4? */

/*     glBindVertexArray(0);     */
/* } */

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
