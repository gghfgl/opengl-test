#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

glm::vec2 worldToScreen(int32 x, int32 y, uint32 tileSize);
glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize);
glm::vec2 screenToNDC(float32 x, float32 y, uint32 screenWidth, uint32 screenHeight);

std::map<std::string, Shader> SHADER_CACHE;
std::map<std::string, Texture2D> TEXTURE_CACHE;

enum GameState {
    GAME_EXIT,
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

struct Game {
    GameState state;
    uint32 width, height;
    glm::vec2 origin;
    glm::uvec2 world;

    std::vector<Sprite> testList;
    Sprite hoveredSprite;

    float32 grid[40];
    uint32 gridVAO;
};

Game InitGameAndLoadAssets(uint32 width, uint32 height) {
    SHADER_CACHE.clear();
    TEXTURE_CACHE.clear();

    // Load and configure shaders.
    Shader spriteShader = LoadShaderFromFile("..\\shaders\\sprite.glsl");
    UseShader(spriteShader.ID);
    SetShaderInteger(spriteShader.ID, "image", 0); // @improve: Investigate
    AddShaderToCache(spriteShader, "sprite", SHADER_CACHE);

    Shader lineShader = LoadShaderFromFile("..\\shaders\\line.glsl");
    //UseShader(lineShader.ID);
    AddShaderToCache(lineShader, "line", SHADER_CACHE);

    // Load textures.
    Texture2D texBlock = LoadTextureFromFile("..\\assets\\block.png", true);
    AddTextureToCache(texBlock, "block", TEXTURE_CACHE);

    Texture2D texBlock2 = LoadTextureFromFile("..\\assets\\block2.png", true);
    AddTextureToCache(texBlock2, "block2", TEXTURE_CACHE);

    Texture2D texBlock3 = LoadTextureFromFile("..\\assets\\block3.png", true);
    AddTextureToCache(texBlock3, "block3", TEXTURE_CACHE);

    Texture2D texBlock4 = LoadTextureFromFile("..\\assets\\block4.png", true);
    AddTextureToCache(texBlock4, "block4", TEXTURE_CACHE);

    Texture2D texBlock5 = LoadTextureFromFile("..\\assets\\block5.png", true);
    AddTextureToCache(texBlock5, "block5", TEXTURE_CACHE);

    Game game;
    game.state = GAME_ACTIVE;
    game.width = width;
    game.height = height;

    // Tests
    game.world.x = 4;
    game.world.y = 4;
    uint32 tileSize = 100;
    game.origin = glm::vec2((float32)(width/2), (float32)(height/2));
    //game.origin = glm::vec2((float32)(width), (float32)(height));
    //game.origin = glm::vec2(0.0f);
    game.testList.clear();

    for (uint32 y=0; y<game.world.y; y++) {
        for (uint32 x=0; x<game.world.x; x++) {
            glm::vec2 screenPos = worldToScreen((int32)x, (int32)y, tileSize);
            game.testList.push_back(InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block"),
                                               glm::vec2(game.origin.x + screenPos.x, game.origin.y + screenPos.y),
                                               glm::vec2((float32)tileSize),
                                               glm::vec3(1.0f)));
        }
    }

    game.hoveredSprite = InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block5"),
                                   glm::vec2(0.0f),
                                   glm::vec2((float32)tileSize),
                                   glm::vec3(1.0f));

    /* // Test draw isometric debug grid */
    uint32 x = 0;
    while (x < 40) {
        glm::vec2 screenPosI = worldToScreen((int32)x, 0, 100/4);
        glm::vec2 screenPosJ = worldToScreen((int32)x, 50, 100/4);

        // miss game origin offset on screenToNDC
        glm::vec2 i = screenToNDC((screenPosI.x + game.origin.x),
                                  (screenPosI.y + game.origin.y),
                                  game.width, game.height);
        glm::vec2 j = screenToNDC((screenPosJ.x + game.origin.x),
                                  (screenPosJ.y + game.origin.y),
                                  game.width, game.height);
        printf("i=%.1f,%.1f | %.1f,%.1f\n", screenPosI.x, screenPosI.y, i.x, i.y);
        printf("j=%.1f,%.1f | %.1f,%.1f\n", screenPosJ.x, screenPosJ.y, j.x, j.y);
        printf("------------------------\n");

        game.grid[x] = i.x;
        x++;
        game.grid[x] = i.y;
        x++;
        game.grid[x] = j.x;
        x++;
        game.grid[x] = j.y;
        x++;
    }

    game.gridVAO = BindVertexArraysFloat(game.grid, sizeof(game.grid), 2);
    
    /* glGenVertexArrays(1, &game.gridVAO); */
    /* glBindVertexArray(game.gridVAO); */

    /* uint32 VBO; */
    /* glGenBuffers(1, &VBO); */

    /* glBindBuffer(GL_ARRAY_BUFFER, VBO); */
    /* glBufferData(GL_ARRAY_BUFFER, sizeof(game.grid), game.grid, GL_STATIC_DRAW); */

    /* // vertex positions */
    /* glEnableVertexAttribArray(0); */
    /* glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float32), (void*)0); */
    
    /* glBufferSubData(GL_ARRAY_BUFFER, 0, 40 * sizeof(float32), game.grid); */
    /* glBindVertexArray(0); */

    return game;
}

void ProcessInput(Game* game, Camera* camera, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 deltaTime) {
    if (mouse->scrollOffsetY != 0.0f) {
        if (GetMouseScrollOffsetY(mouse) > 0.0f) {
            camera->zoom += .05f;
        } else {
            camera->zoom -= .05f;
        }
    }

    float32 velocity = camera->speed * 1.0f * deltaTime;
    if (mouse->leftButton) {
        UpdateMouseOffsets(mouse);
        camera->position.x -= mouse->offsetX / camera->zoom * velocity;
        camera->position.y += mouse->offsetY / camera->zoom * velocity;
    }

    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_ESCAPE])
        game->state = GAME_EXIT;
}

void SoundSample(Game* game) {
    // ...
}

glm::vec2 worldToScreen(int32 x, int32 y, uint32 tileSize) {
    glm::vec2 screenPos(0.0f);
    screenPos.x = (x * 0.5f * tileSize) + (y * -0.5f * tileSize) - (0.5f * tileSize);
    screenPos.y = (x * 0.25f * tileSize) + (y * 0.25f * tileSize);

    return screenPos;
}

glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize) {
    float32 wx = (y / (tileSize/4) + x / (tileSize/2)) * 0.5f;
    float32 wy = (y / (tileSize/4) - x / (tileSize/2)) * 0.5f;

    glm::ivec2 world(0);
    world.x = (int32)glm::floor(wx);
    world.y = (int32)glm::floor(wy);

    return world;
}

glm::vec2 screenToNDC(float32 x, float32 y, uint32 screenWidth, uint32 screenHeight) {
    glm::vec2 ndc = glm::vec2(0.0f);

    ndc.x = x / screenWidth * 2 - 1;
    ndc.y = -(y / screenHeight * 2 - 1);

    return ndc;
}

void UpdateAndRender(Game* game, Camera* camera, SpriteRenderer* renderer, Plateform_MouseEvent* mouse) {
    /* printf("zoom %.1f\n", camera->zoom); */
    /* printf("cam pos %.1f | %.1f\n", camera->position.x, camera->position.y); */
    /* printf("mouse pos %.1f | %.1f\n", mouse->posX, mouse->posY); */

	// @improve: Should avoid as many drawcalls! Batch rendering?
    for(std::size_t i = 0; i < game->testList.size(); ++i) {
        DrawSprite(renderer, &game->testList[i], GetShaderFromCache(SHADER_CACHE, "sprite"), camera);
    }

    // Convert screen mouse coordinate to isometric coord postition
    glm::vec2 mouseWorld = screenToWorld((float32)((mouse->posX - game->origin.x) / camera->zoom + camera->position.x),
                                         (float32)((mouse->posY - game->origin.y) / camera->zoom + camera->position.y),
                                         (uint32)game->hoveredSprite.size.x);
    //printf("mouse world %.1f/%.1f\n", mouseWorld.x, mouseWorld.y);

    // Check if
    if ((mouseWorld.x >= 0 && mouseWorld.y >= 0) && (mouseWorld.x < game->world.x && mouseWorld.y < game->world.y)) {
        // Convert back hovered isometric position to screen coordinate
        glm::vec2 hoveredPos = worldToScreen((int32)mouseWorld.x,
                                             (int32)mouseWorld.y,
                                             (uint32)game->hoveredSprite.size.x);
        game->hoveredSprite.position.x = hoveredPos.x + game->origin.x;
        game->hoveredSprite.position.y = hoveredPos.y + game->origin.y;

        DrawSprite(renderer, &game->hoveredSprite, GetShaderFromCache(SHADER_CACHE, "sprite"), camera);
    }

    // Test reference grid isometric
    float32 xPos = 0.5f * camera->width + camera->position.x;
    float32 yPos = 0.5f * camera->height + camera->position.y;
    float32 left = -camera->width/(2*camera->zoom) + xPos;
    float32 right = camera->width/(2*camera->zoom) + xPos;
    float32 top = -camera->height/(2*camera->zoom) + yPos;
    float32 bottom = camera->height/(2*camera->zoom) + yPos;
    // HERE!!!!! cannot make this projection working with camera movement!
    //glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    Shader lineShader = GetShaderFromCache(SHADER_CACHE, "line");
    UseShader(lineShader.ID);
    SetShaderMatrix4(lineShader.ID, "projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(.1f, .3f, 1.0f));
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.0f));
    SetShaderMatrix4(lineShader.ID, "model", model);
    //SetShaderMatrix4(lineShader.ID, "model", glm::mat4(1.0f));

    SetShaderVector4f(lineShader.ID, "color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));


    glLineWidth(3.0f);
    glBindVertexArray(game->gridVAO);
    glDrawArrays(GL_LINES, 0, 40); // maybe 4?

    glBindVertexArray(0);
}
