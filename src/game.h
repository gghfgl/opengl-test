#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

enum GameState {
    GAME_EXIT,
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

// @improve: Meh ... pass from the main call?
// Initial size of the player paddle
//const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
//const float32 PLAYER_VELOCITY(500.0f);

std::map<std::string, Shader> SHADER_CACHE;
std::map<std::string, Texture2D> TEXTURE_CACHE;

struct Game {
    GameState state;
    uint32 width, height;

    // @delete: Should be remove after tests. Nothing to do there!
    Sprite test;
};

void InitGameAndLoadAssets(Game* game, uint32 width, uint32 height) {
    SHADER_CACHE.clear();
    TEXTURE_CACHE.clear();

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

    game->state = GAME_ACTIVE;
    game->width = width;
    game->height = height;

    /* glm::vec2 playerPos = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y); */
    /* game->Player = InitGameObject(GetTextureFromCache(textureCache, "paddle"), playerPos, PLAYER_SIZE, glm::vec3(1.0f)); */
    game->test = InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block"), glm::vec2(100.0f, 100.0f), glm::vec2(130.0f, 130.0f), glm::vec3(1.0f));
}

void ProcessInput(Game* game, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 dt) {
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_ESCAPE])
        game->state = GAME_EXIT;
}

void UpdateAndRender(Game* game, SpriteRenderer* renderer) {
    DrawSprite(renderer, &game->test, GetShaderFromCache(SHADER_CACHE, "sprite"));

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
