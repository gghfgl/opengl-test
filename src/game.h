#pragma once

#include <vector>

#include "sprite_renderer.h"
#include "game_level.h"
#include "game_object.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

// @improve: Add to Game struct.
SpriteRenderer Renderer;
std::map<std::string, Texture2D> TextureCache;
std::map<std::string, Shader> ShaderCache;

// @improve: Meh ... pass from the main call?
// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float32 PLAYER_VELOCITY(500.0f);

struct Game {
    GameState State;
    bool Keys[1024];
    uint32 Width, Height;
    SpriteRenderer SpriteRenderer;

    // @improve: Meh ....
    std::vector<GameLevel> Levels;
    uint32 CurrentLevelIdx;
    GameObject      Player;
    GameObject      Floor;
};

Game InitGame(uint32 width, uint32 height) {
    Game game;
    game.State = GAME_ACTIVE;
    game.Width = width;
    game.Height = height;

    // load shaders
    Shader spriteShader;
    LoadShaderFromFile(&spriteShader, "..\\shaders\\sprite_vertex.glsl", "..\\shaders\\sprite_fragment.glsl");
    AddShaderToCache(spriteShader, "sprite", ShaderCache);

    // configure shader
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float32>(game.Width), static_cast<float32>(game.Height), 0.0f, -1.0f, 1.0f);

    UseShader(spriteShader);
    SetShaderInteger(spriteShader, "image", 0);
    SetShaderMatrix4(spriteShader, "projection", projection);

    // set render-specific controls
    SpriteRenderer SpriteRenderer = InitSpriteRenderer(spriteShader);
    InitSpriteBuffer(&SpriteRenderer);
    game.SpriteRenderer = SpriteRenderer;

    // load textures
    Texture2D texFace = InitTexture(0, 0, true);
    LoadTextureFromFile(&texFace, "..\\assets\\awesomeface.png");
    AddTextureToCache(texFace, "face", TextureCache);

    Texture2D texBackground = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBackground, "..\\assets\\background.jpg");
    AddTextureToCache(texBackground, "background", TextureCache);

    Texture2D texBrick = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBrick, "..\\assets\\brick.png");
    AddTextureToCache(texBrick, "brick", TextureCache);

    Texture2D texBrickSolid = InitTexture(0, 0, false);
    LoadTextureFromFile(&texBrickSolid, "..\\assets\\brick_solid.png");
    AddTextureToCache(texBrickSolid, "brick_solid", TextureCache);

    Texture2D texPaddle = InitTexture(0, 0, true);
    LoadTextureFromFile(&texPaddle, "..\\assets\\paddle.png");
    AddTextureToCache(texPaddle, "paddle", TextureCache);

    Texture2D texFloor = InitTexture(0, 0, true);
    LoadTextureFromFile(&texFloor, "..\\assets\\floor.png");
    AddTextureToCache(texFloor, "floor", TextureCache);

    // @improve: Meh ...
    // load levels
    GameLevel levelOne = LoadGameLevelFromFile("..\\assets\\level1");
    InitGameLevelObjects(&levelOne, width, height / 2, TextureCache);
    GameLevel levelTwo = LoadGameLevelFromFile("..\\assets\\level2");
    InitGameLevelObjects(&levelTwo, width, height / 2, TextureCache);
    GameLevel levelThree = LoadGameLevelFromFile("..\\assets\\level3");
    InitGameLevelObjects(&levelThree, width, height / 2, TextureCache);
    GameLevel levelFour = LoadGameLevelFromFile("..\\assets\\level4");
    InitGameLevelObjects(&levelFour, width, height / 2, TextureCache);

    game.Levels.push_back(levelOne);
    game.Levels.push_back(levelTwo);
    game.Levels.push_back(levelThree);
    game.Levels.push_back(levelFour);
    game.CurrentLevelIdx = 0;

    glm::vec2 playerPos = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y);
    game.Player = InitGameObject(GetTextureFromCache(TextureCache, "paddle"), playerPos, PLAYER_SIZE, glm::vec3(1.0f));

    game.Floor = InitGameObject(GetTextureFromCache(TextureCache, "floor"), glm::vec2(100.0f, 100.0f), glm::vec2(130.0f, 130.0f), glm::vec3(1.0f));
    
    return game;
}

void ProcessInput(Game* game, float32 dt) {
    if (game->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (game->Keys[GLFW_KEY_A])
        {
            if (game->Player.Position.x >= 0.0f)
                game->Player.Position.x -= velocity;
        }
        if (game->Keys[GLFW_KEY_D])
        {
            if (game->Player.Position.x <= game->Width - game->Player.Size.x)
                game->Player.Position.x += velocity;
        }
    }
}

void Update(Game* game, float32 dt) {
    // ...
}

void Render(Game* game, SpriteRenderer renderer) {
    DrawSprite(renderer, game->Floor.Sprite, game->Floor.Position, game->Floor.Size, game->Floor.Rotation, game->Floor.Color);

    /* if(game->State == GAME_ACTIVE) { */
    /*     Texture2D texBackground = GetTextureFromCache(TextureCache, "background"); */
    /*     DrawSprite(game->Renderer, texBackground, glm::vec2(0.0f, 0.0f), glm::vec2(game->Width, game->Height), 0.0f, glm::vec3(1.0f)); */

    /*     for (GameObject &tile : game->Levels[game->CurrentLevelIdx].Bricks) */
    /*         if (!tile.Destroyed) { */
    /*             DrawSprite(game->Renderer, tile.Sprite, tile.Position, tile.Size, tile.Rotation, tile.Color); */
    /*         } */

    /*     DrawSprite(game->Renderer, game->Player.Sprite, game->Player.Position, game->Player.Size, game->Player.Rotation, game->Player.Color); */
    /* } */

    /* Texture2D texFace = GetTextureFromCache(TextureCache, "face"); */
    /* DrawSprite(game->Renderer, texFace, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)); */
}
