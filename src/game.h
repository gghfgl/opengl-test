#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

// @improve: Add to Game struct.
std::map<std::string, Texture2D> TextureCache;
std::map<std::string, Shader> ShaderCache;

// @improve: Meh ... pass from the main call?
// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float32 PLAYER_VELOCITY(500.0f);

struct Game {
    GameState state;
    // bool keys[1024]; @delete: Unused?
    uint32 width, height;

    std::map<std::string, Texture2D> textureCache;
    std::map<std::string, Shader> shaderCache;

    // @delete: Should be remove after tests. Nothing to do there!
    Sprite* test;
};

Game* InitGame(uint32 width, uint32 height) {
    std::map<std::string, Texture2D> textureCache;
    textureCache.clear();
    std::map<std::string, Shader> shaderCache;
    shaderCache.clear();
    
    // load shaders
    Shader spriteShader;
    LoadShaderFromFile(&spriteShader, "..\\shaders\\sprite_vertex.glsl", "..\\shaders\\sprite_fragment.glsl");
    AddShaderToCache(spriteShader, "sprite", shaderCache);

    // configure shader
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float32>(width), static_cast<float32>(height), 0.0f, -1.0f, 1.0f);
    UseShader(spriteShader);
    SetShaderInteger(spriteShader, "image", 0);
    SetShaderMatrix4(spriteShader, "projection", projection);

    // load textures
    Texture2D texFace = InitTexture(0, 0, true);
    LoadTextureFromFile(&texFace, "..\\assets\\awesomeface.png");
    AddTextureToCache(texFace, "face", textureCache);

    Texture2D texBackground = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBackground, "..\\assets\\background.jpg");
    AddTextureToCache(texBackground, "background", textureCache);

    Texture2D texBrick = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBrick, "..\\assets\\brick.png");
    AddTextureToCache(texBrick, "brick", textureCache);

    Texture2D texBrickSolid = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBrickSolid, "..\\assets\\brick_solid.png");
    AddTextureToCache(texBrickSolid, "brick_solid", textureCache);

    Texture2D texPaddle = InitTexture(0, 0, true);
    LoadTextureFromFile(&texPaddle, "..\\assets\\paddle.png");
    AddTextureToCache(texPaddle, "paddle", textureCache);

    Texture2D texFloor = InitTexture(0, 0, true);
    LoadTextureFromFile(&texFloor, "..\\assets\\floor.png");
    AddTextureToCache(texFloor, "floor", textureCache);

    Game* game = new Game;
    game->state = GAME_ACTIVE;
    game->width = width;
    game->height = height;
    game->shaderCache = shaderCache;
    game->textureCache = textureCache;
    /* glm::vec2 playerPos = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y); */
    /* game.Player = InitGameObject(GetTextureFromCache(textureCache, "paddle"), playerPos, PLAYER_SIZE, glm::vec3(1.0f)); */
    game->test = InitSprite(GetTextureFromCache(textureCache, "floor"), glm::vec2(100.0f, 100.0f), glm::vec2(130.0f, 130.0f), glm::vec3(1.0f));
    
    return game;
}

void ProcessInput(Game* game, KeyboardEvent* keyboard, MouseEvent* mouse, float32 dt) {
    /* if (keyboard->isPressed[keyboard::CRAP_KEY_ESCAPE]) */
    /*     glfwSetWindowShouldClose(window->context, true); // OPENGL CODE BAHHHHhh! */
}

void Update(Game* game, float32 dt) {
    // ...
}

void RenderGame(Game* game, SpriteRenderer* renderer) {
    DrawSprite(renderer, game->test, GetShaderFromCache(game->shaderCache, "sprite"));

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
