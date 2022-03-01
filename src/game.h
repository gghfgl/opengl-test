#pragma once

#include <vector>

#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"

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
};

Game InitGameAndLoadAssets(uint32 width, uint32 height) {
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

    Texture2D texBlock2 = LoadTextureFromFile("..\\assets\\block2.png", true);
    AddTextureToCache(texBlock2, "block2", TEXTURE_CACHE);

    Texture2D texBlock3 = LoadTextureFromFile("..\\assets\\block3.png", true);
    AddTextureToCache(texBlock3, "block3", TEXTURE_CACHE);

    Game game;
    game.state = GAME_ACTIVE;
    game.width = width;
    game.height = height;

    return game;
}

void ProcessInput(Game* game, Plateform_KeyboardEvent* keyboard, Plateform_MouseEvent* mouse, float32 dt) {
    if (keyboard->isPressed[KEYBOARD::CRAP_KEY_ESCAPE])
        game->state = GAME_EXIT;
}

void SoundSample(Game* game) {
    // ...
}

/* glm::vec2 worldToScreen(glm::vec2 originWorld, uint32 x, uint32 y, uint32 size) { */
/*     float32 screenX = (originWorld.x * size) + (x - y) * (size / 2); */
/*     float32 screenY = (originWorld.y * size) + (x + y) * (size / 2); */

/*     return glm::vec2(screenX, screenY); */
/* } */

glm::vec2 worldToScreen(uint32 x, uint32 y, uint32 tileSize) {
    glm::vec2 screenPos(0.0f);

    float32 posX = (float32)(x*tileSize);
    float32 posY = (float32)(y*tileSize);

    screenPos.x = (posX - posY) / 2;
    screenPos.y = (posX + posY) / 4;

    return screenPos;
}

glm::vec2 worldToScreen2(uint32 x, uint32 y, uint32 tileSize) {
    glm::vec2 screenPos(0.0f);

    screenPos.x = ((float32)(x * 0.5 * tileSize) + (float32)(y * (-0.5) * tileSize)) - (float32)(tileSize / 2);
    screenPos.y = (float32)(x * 0.25 * tileSize) + (float32)(y * 0.25 * tileSize);

    return screenPos;
}

glm::vec2 screenToWorld(float32 x, float32 y, uint32 tileSize) {
    /* x = 0.5 * ( screenX / tileWidthHalf + screenY / tileHeightHalf) */
    /* y = 0.5 * (-screenX / tileWidthHalf + screenY / tileHeightHalf) */
    glm::vec2 worldPos(0.0f);

    worldPos.x = (float32)(0.5 * (x / (tileSize/2) + y / (tileSize/4)));
    worldPos.y = (float32)(0.5 * (-x / (tileSize/2) + y / (tileSize/4)));

    return worldPos;
}

void UpdateAndRender(Game* game, SpriteRenderer* renderer, Plateform_MouseEvent* mouse) {
    std::vector<Sprite> testList;
    testList.clear();

    uint32 sizeX = 10;
    uint32 sizeY = 14;
    uint32 spriteSize = 100;
    //glm::vec2 cellCoordIso = glm::vec2(game->width / spriteSize, game->height / spriteSize); // 19,14
    glm::vec2 origin = glm::vec2((float32)(game->width/2), (float32)(game->height/3));
    //glm::vec2 origin = glm::vec2(0.0f);


    //printf("mouse %.1f/%.1f\n", mouse->posX, mouse->posY);
    // 2. get cell from world space the mouse pos is in: (mouse.x / spriteSize) (mouse.y / spriteSze)
    glm::vec2 cell = glm::vec2((float32)(mouse->posX / spriteSize), (float32)(mouse->posY / spriteSize));

    // 3. get offset into the cell: (mouse.x % spriteSize) (mouse.y % spriteSize)
    //glm::vec2 cellOffset = glm::vec2(mouse->posX % spriteSize, mouse->posY % spriteSize);
    // .. Current cell hovered: (cell.x * spriteSize) (cell.y * spriteSize)
    // 4. Selected cell: (cell.y - origin.y) + (cell.x - origin.x)
    //                   (cell.y - origin.y) - (cell.x - origin.x)
    //glm::vec2 selectCell = glm::vec2((cell.y - origin.y) + (cell.x - origin.x), (cell.y - origin.y) - (cell.x - origin.x));

    for (uint32 y=0; y<sizeY; y++) {
        for (uint32 x=0; x<sizeX; x++) {
            glm::vec2 screenPos = worldToScreen2(x, y, spriteSize);
            testList.push_back(InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block"),
                                          glm::vec2(origin.x + screenPos.x, origin.y + screenPos.y),
                                          glm::vec2((float32)spriteSize),
                                          glm::vec3(1.0f)));
        }
    }



// @improve: Should avoid as many drawcalls! Batch rendering?
    for(std::size_t i = 0; i < testList.size(); ++i)
        DrawSprite(renderer, &testList[i], GetShaderFromCache(SHADER_CACHE, "sprite"));

    // 5. selected World = selectedCell worldToScreen convert
    //printf("mouse %.1f/%.1f\n", mouse->posX, mouse->posY);
    //printf("cell %d/%d\n", (uint32)(cell.x), (uint32)(cell.y));
    glm::vec2 test = screenToWorld((float32)(mouse->posX - origin.x), (float32)(mouse->posY - origin.y), spriteSize);
    printf("world %.1f/%.1f\n", test.x, test.y);
    glm::vec2 selectedCellPos = worldToScreen2((uint32)(test.x), (uint32)(test.y), spriteSize);
    printf("screen %.1f/%.1f\n", selectedCellPos.x, selectedCellPos.y);
    // 6. Draw block 2 at this pos.
    Sprite selected = InitSprite(GetTextureFromCache(TEXTURE_CACHE, "block2"),
                                 glm::vec2(origin.x + selectedCellPos.x, origin.y + selectedCellPos.y),
                                 glm::vec2((float32)spriteSize),
                                 glm::vec3(1.0f));
    DrawSprite(renderer, &selected, GetShaderFromCache(SHADER_CACHE, "sprite"));
}
