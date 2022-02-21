#pragma once

#include "sprite_renderer.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

// Game-related State data
SpriteRenderer Renderer;
std::map<std::string, Texture2D> TextureCache;
std::map<std::string, Shader> ShaderCache;

struct Game {
    GameState State;
    bool Keys[1024];
    uint32 Width, Height;
    SpriteRenderer Renderer;
};

Game InitGame(uint32 width, uint32 height) {
    Game game;
    game.State = GAME_ACTIVE;
    game.Width = width;
    game.Height = height;

    // load shaders
    Shader shader;
    LoadShaderFromFile(&shader, "..\\glsl\\sprite_vertex.glsl", "..\\glsl\\sprite_fragment.glsl");
    AddShaderToCache(shader, "sprite", ShaderCache);

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float32>(game.Width), static_cast<float32>(game.Height), 0.0f, -1.0f, 1.0f);
    UseShader(shader);
    SetShaderInteger(shader, "image", 0);
    SetShaderMatrix4(shader, "projection", projection);

    // set render-specific controls
    SpriteRenderer renderer = InitSpriteRenderer(shader);
    InitQuadVAO(&renderer);
    game.Renderer = renderer;

    // load textures
    Texture2D texture = InitTexture(0, 0, true);
    LoadTextureFromFile(&texture, "..\\assets\\awesomeface.png");
    AddTextureToCache(texture, "face", TextureCache);

    return game;
}

void ProcessInput(float32 dt) {
    // ...
}

void Update(float32 dt) {
    // ...
}

void Render(SpriteRenderer renderer) {
    Texture2D texture = GetTextureFromCache(TextureCache, "face");
    DrawSprite(renderer, texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
