#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

glm::vec2 worldToScreen(uint32 x, uint32 y, uint32 tileSize);
glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize);

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
};

Game InitGameAndLoadAssets(uint32 width, uint32 height) {
    SHADER_CACHE.clear();
    TEXTURE_CACHE.clear();

    // Load and configure shaders.
    Shader spriteShader = LoadShaderFromFile("..\\shaders\\sprite.glsl");
    UseShader(spriteShader.ID);
    SetShaderInteger(spriteShader.ID, "image", 0);
    AddShaderToCache(spriteShader, "sprite", SHADER_CACHE);

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
    game.world.x = 20;
    game.world.y = 20;
    uint32 spriteSize = 100;
    game.origin = glm::vec2((float32)(width/2), (float32)(height/2));
    //game.origin = glm::vec2((float32)(width), (float32)(height));
    //game.origin = glm::vec2(0.0f);
    game.testList.clear();

    for (uint32 y=0; y<game.world.y; y++) {
        for (uint32 x=0; x<game.world.x; x++) {
            glm::vec2 screenPos = worldToScreen(x, y, spriteSize);
            game.testList.push_back(InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block"),
                                               glm::vec2(game.origin.x + screenPos.x, game.origin.y + screenPos.y),
                                               glm::vec2((float32)spriteSize),
                                               glm::vec3(1.0f)));
        }
    }

    game.hoveredSprite = InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block5"),
                                   glm::vec2(0.0f),
                                   glm::vec2((float32)spriteSize),
                                   glm::vec3(1.0f));
    
    return game;
}

void ProcessInput(Game* game, Camera* camera, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 deltaTime) {
    if (mouse->scrollOffsetY != 0.0f) {
        if (GetMouseScrollOffsetY(mouse) > 0.0f) {
            camera->zoom += .05f;
            //UpdateCameraPositionZAxis(camera, CAMERA_FORWARD, deltaTime);
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

glm::vec2 worldToScreen(uint32 x, uint32 y, uint32 tileSize) {
    glm::vec2 screenPos(0.0f);
    screenPos.x = ((float32)(x * 0.5 * tileSize) + (float32)(y * (-0.5) * tileSize)) - (float32)(tileSize / 2);
    screenPos.y = (float32)(x * 0.25 * tileSize) + (float32)(y * 0.25 * tileSize);

    return screenPos;
}

glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize) {
    int32 wx = (int32)(0.5 * (x / (tileSize/2) + y / (tileSize/4)));
    int32 wy = (int32)(0.5 * (-x / (tileSize/2) + y / (tileSize/4)));

    glm::ivec2 world(0);
    world.x = (int32)glm::floor(wx);
    world.y = (int32)glm::floor(wy);

    return world;
}

void UpdateAndRender(Game* game, Camera* camera, SpriteRenderer* renderer, Plateform_MouseEvent* mouse) {
    printf("zoom %.1f\n", camera->zoom);
    printf("cam pos %.1f | %.1f\n", camera->position.x, camera->position.y);
    printf("mouse pos %.1f | %.1f\n", mouse->posX, mouse->posY);

	// @improve: Should avoid as many drawcalls! Batch rendering?
    for(std::size_t i = 0; i < game->testList.size(); ++i) {
        DrawSprite(renderer, &game->testList[i], GetShaderFromCache(SHADER_CACHE, "sprite"), camera);
    }

    // Mouse world position
    glm::vec2 mouseWorld = screenToWorld((float32)((mouse->posX - game->origin.x) / camera->zoom + camera->position.x),
                                         (float32)((mouse->posY - game->origin.y) / camera->zoom + camera->position.y),
                                         (uint32)game->hoveredSprite.size.x);
    //printf("mouse world %.1f/%.1f\n", mouseWorld.x, mouseWorld.y);

    // Render hovered sprite
    if ((mouseWorld.x >= 0 && mouseWorld.y >= 0) && (mouseWorld.x < game->world.x && mouseWorld.y < game->world.y)) {
        glm::vec2 hoveredPos = worldToScreen((uint32)mouseWorld.x,
                                             (uint32)mouseWorld.y,
                                             (uint32)game->hoveredSprite.size.x);
        game->hoveredSprite.position.x = hoveredPos.x + game->origin.x;
        game->hoveredSprite.position.y = hoveredPos.y + game->origin.y;

        DrawSprite(renderer, &game->hoveredSprite, GetShaderFromCache(SHADER_CACHE, "sprite"), camera);
    }
}
