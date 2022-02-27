#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

// @improve: Make a custom memory allocator to be able to track real memory usage.
struct MemoryTracker {
    std::map<std::string, Shader> *shaderCache;
    std::map<std::string, Texture2D> *textureCache;
};

void PrintMemoryTrackerUsage(MemoryTracker* mt) {
    uint64 sCache = mt->shaderCache->size() * (sizeof(std::string) + sizeof(Shader));
    uint64 tCache = mt->textureCache->size() * (sizeof(std::string) + sizeof(Texture2D));

    Log::info("MEMORY: shader_cache: %d Bytes | texture_cache: %d Bytes\n", sCache, tCache);
}

MemoryTracker* MEM_TRACK;

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

    // @delete: Should be remove after tests. Nothing to do there!
    std::vector<Sprite> testList;
};

Game InitGameAndLoadAssets(uint32 width, uint32 height) {
    SHADER_CACHE.clear();
    TEXTURE_CACHE.clear();
    MEM_TRACK = new MemoryTracker;
    MEM_TRACK->shaderCache = &SHADER_CACHE;
    MEM_TRACK->textureCache = &TEXTURE_CACHE;

    // Set projection.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float32>(width), static_cast<float32>(height), 0.0f, -1.0f, 1.0f);
    
    // Load and configure shaders.
    Shader spriteShader = LoadShaderFromFile("..\\shaders\\sprite.glsl");
    UseShader(spriteShader.ID);
    SetShaderInteger(spriteShader.ID, "image", 0);
    SetShaderMatrix4(spriteShader.ID, "projection", projection);
    AddShaderToCache(spriteShader, "sprite", SHADER_CACHE);

    // Load textures.
    Texture2D texBlock = LoadTextureFromFile("..\\assets\\block.png", true);
    AddTextureToCache(texBlock, "block", TEXTURE_CACHE);

    Game game;
    game.state = GAME_ACTIVE;
    game.width = width;
    game.height = height;
    game.testList.clear();

    uint32 size = 20;
    float32 spriteSize = 100.0f;
    float32 startPosX = (float32)(width/2) - (spriteSize/2);
    float32 startPosY = 300.0f;
    for (uint32 i=0; i<size; i++) {
        for (uint32 y=0; y<size; y++) {
            float32 posX = startPosX + (float32)(spriteSize/2 * y);
            float32 posY = startPosY + (float32)((spriteSize/4) * y);
            game.testList.push_back(InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block"),
                                               glm::vec2(posX, posY),
                                               glm::vec2(spriteSize),
                                               glm::vec3(1.0f)));
        }

        startPosX -= (spriteSize/2);
        startPosY += (spriteSize/4);
    }

    return game;
}

void ProcessInput(Game* game, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 dt) {
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_ESCAPE])
        game->state = GAME_EXIT;
}

void UpdateAndRender(Game* game, SpriteRenderer* renderer) {
    // @improve: Should avoid as many drawcalls! Batch rendering?
    for(std::size_t i = 0; i < game->testList.size(); ++i)
        DrawSprite(renderer, &game->testList[i], GetShaderFromCache(SHADER_CACHE, "sprite"));

    /* if(game->State == GAME_ACTIVE) { */
    /*     Texture2D texBackground = GetTextureFromCache(textureCache, "background"); */
    /*     DrawSprite(game->Renderer, texBackground, glm::vec2(0.0f, 0.0f), glm::vec2(game->Width, game->Height), 0.0f, glm::vec3(1.0f)); */

    /*     for (GameObject &tile : game->Levels[game->CurrentLevelIdx].Bricks) */
    /*         if (!tile.Destroyed) { */
    /*             DrawSprite(game->Renderer, tile.Sprite, tile.Position, tile.Size, tile.Rotation, tile.Color); */
    /*         } */

    /*     DrawSprite(game->Renderer, game->Player.Sprite, game->Player.Position, game->Player.Size, game->Player.Rotation, game->Player.Color); */
    /* } */

    /* Texture2D texFace = GetTextureFromCache(textureCache, "face"); */
    /* DrawSprite(game->Renderer, texFace, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)); */
}

void SoundSaple(Game* game) {
    // ...
}
