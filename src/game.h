#pragma once

#include "sprite_renderer.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

// Game-related State data
SpriteRenderer *Renderer;
std::map<std::string, Texture2D> TextureCache;
std::map<std::string, Shader> ShaderCache;

struct Game {
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
        // ...
    }

    ~Game() {
        // ...
    }

    void Init() {
        // load shaders
        Shader shader;
        LoadShaderFromFile(&shader, "..\\glsl\\sprite_vertex.glsl", "..\\glsl\\sprite_fragment.glsl");
        AddShaderToCache(shader, "sprite", ShaderCache);

        // configure shaders
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
        UseShader(shader);
        SetShaderInteger(shader, "image", 0);
        SetShaderMatrix4(shader, "projection", projection);

        // set render-specific controls
        Renderer = InitSpriteRenderer(shader);

        // load textures
        Texture2D texture = InitTexture(0, 0, true);
        LoadTextureFromFile(&texture, "..\\assets\\awesomeface.png");
        AddTextureToCache(texture, "face", TextureCache);
    }

    void ProcessInput(float dt) {
        // ...
    }

    void Update(float dt) {
        // ...
    }

    void Render() {
        Texture2D texture = GetTextureFromCache(TextureCache, "face");
        DrawSprite(Renderer, texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
