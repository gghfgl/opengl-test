#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

glm::vec2 worldToScreen(uint32 x, uint32 y, uint32 tileSize);
glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize);

std::map<std::string, Shader> SHADER_CACHE;
std::map<std::string, Texture2D> TEXTURE_CACHE;

enum CameraDirection {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
};

struct Camera {
    glm::mat4 projection;
    glm::vec2 position;
    glm::mat4 zoomMatrix;
    glm::vec2 aspect;

    float32 speed;
    float32 zoom;
};

enum GameState {
    GAME_EXIT,
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

struct Game {
    GameState state;
    uint32 width, height;

    Camera camera;
    glm::vec2 origin;
    glm::uvec2 world;
    std::vector<Sprite> testList;
    Sprite hoveredSprite;
};

Game InitGameAndLoadAssets(uint32 width, uint32 height) {
    SHADER_CACHE.clear();
    TEXTURE_CACHE.clear();

    // Set projection.
    Camera camera;
    camera.position = glm::vec2(0.0f);
    camera.speed = 20.0f;
    camera.zoom = 1.0f;
    camera.zoomMatrix = glm::mat4(1.0f);
    camera.aspect = glm::vec2(0.0f);

    camera.projection = glm::ortho(0.0f, (float32)width, (float32)height, 0.0f, -10.0f, 1.0f);
    
    // Load and configure shaders.
    Shader spriteShader = LoadShaderFromFile("..\\shaders\\sprite.glsl");
    UseShader(spriteShader.ID);
    SetShaderInteger(spriteShader.ID, "image", 0);
    SetShaderMatrix4(spriteShader.ID, "projection", camera.projection);
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
    game.camera = camera;
    game.world.x = 10;
    game.world.y = 14;
    uint32 spriteSize = 100;
    game.origin = glm::vec2((float32)(width/2), (float32)(height/3));
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

void ProcessInput(Game* game, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 dt) {
    // test scroll
    if (mouse->scrollOffsetY != 0.0f) {
        printf("scroll offset %.1f\n", mouse->scrollOffsetY);
        float32 scrollOffset = GetMouseScrollOffsetY(mouse);
        if (scrollOffset > 0.0f)
            game->camera.zoom += .05f;
        else
            game->camera.zoom -= .05f;

        //game->camera.zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(game->camera.zoom));
        //game->camera.projection = glm::ortho(game->camera.zoom, (float32)game->width, (float32)game->height, game->camera.zoom, -10.0f, 1.0f);

    }

    if (mouse->leftButton) {
        UpdateMouseOffsets(mouse);
        game->camera.position.x -= mouse->offsetX;
        game->camera.position.y += mouse->offsetY;
    }

    float32 acceleration = 1.0f;
    float32 velocity = game->camera.speed * acceleration * dt;
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_ESCAPE])
        game->state = GAME_EXIT;
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_LEFT])
        game->camera.position.x += 10.0f * velocity;
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_RIGHT])
        game->camera.position.x -= 10.0f * velocity;
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_UP])
        game->camera.position.y += 10.0f * velocity;
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_DOWN])
        game->camera.position.y -= 10.0f * velocity;
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

void UpdateAndRender(Game* game, SpriteRenderer* renderer, Plateform_MouseEvent* mouse) {
    printf("zoom %.1f\n", game->camera.zoom);
    printf("mous %.1f | %.1f\n", mouse->posX, mouse->posY);

	// @improve: Should avoid as many drawcalls! Batch rendering?
    for(std::size_t i = 0; i < game->testList.size(); ++i) {        
        DrawSprite(renderer, &game->testList[i], GetShaderFromCache(SHADER_CACHE, "sprite"), game->camera.projection, game->camera.position, game->camera.zoomMatrix, game->camera.zoom);
    }

    // Mouse world position
    glm::vec2 mouseWorld = screenToWorld((float32)(mouse->posX - game->origin.x + game->camera.position.x),
                                         (float32)(mouse->posY - game->origin.y + game->camera.position.y),
                                         (uint32)game->hoveredSprite.size.x);
    printf("mouse world %.1f/%.1f\n", mouseWorld.x, mouseWorld.y);

    // Render hovered sprite
    if ((mouseWorld.x >= 0 && mouseWorld.y >= 0) && (mouseWorld.x < game->world.x && mouseWorld.y < game->world.y)) {
        glm::vec2 hoveredPos = worldToScreen((uint32)mouseWorld.x,
                                             (uint32)mouseWorld.y,
                                             (uint32)game->hoveredSprite.size.x);
        game->hoveredSprite.position.x = game->origin.x + hoveredPos.x;
        game->hoveredSprite.position.y = game->origin.y + hoveredPos.y;

        DrawSprite(renderer, &game->hoveredSprite, GetShaderFromCache(SHADER_CACHE, "sprite"), game->camera.projection, game->camera.position, game->camera.zoomMatrix, game->camera.zoom);
    }
}
