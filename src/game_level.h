#pragma once

#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include "game_object.h"

struct GameLevel {
    std::vector<GameObject> Bricks;
    std::vector<std::vector<uint32>> tileData;
};

GameLevel LoadGameLevelFromFile(const char *file) {
    GameLevel level;
    level.Bricks.clear();
    level.tileData.clear(); // @improve: 2 dimensional vector?
    uint32 tileCode;

    std::string line;
    std::ifstream fstream(file);
    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<uint32> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            level.tileData.push_back(row);
        }
        if (level.tileData.size() == 0)
            Log::warn("GAME_LEVEL: Vector tileData is empty\n");
    }

    return level;
}

void InitGameLevelObjects(GameLevel* level, uint32 levelWidth, uint32 levelHeight, std::map<std::string, Texture2D> TextureCache) {
    // calculate dimensions
    uint32 height = level->tileData.size();
    uint32 width = level->tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float32 unit_width = levelWidth / static_cast<float32>(width), unit_height = levelHeight / height; 
    // initialize level tiles based on level->tileData		
    for (uint32 y = 0; y < height; ++y) {
        for (uint32 x = 0; x < width; ++x) {
            // check block type from level data (2D level array)
            if (level->tileData[y][x] == 1) { // solid
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                GameObject obj = InitGameObject(GetTextureFromCache(TextureCache, "brick_solid"), pos, size, glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                level->Bricks.push_back(obj);
            } else if (level->tileData[y][x] > 1) { // non-solid; now determine its color based on level data
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (level->tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (level->tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (level->tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (level->tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                GameObject obj = InitGameObject(GetTextureFromCache(TextureCache, "brick"), pos, size, color);
                level->Bricks.push_back(obj);
            }
        }
    }
}

// @imprive: Mehh ??
// bool GameLevelIsCompleted() {
//     for (GameObject &tile : this->Bricks)
//         if (!tile.IsSolid && !tile.Destroyed)
//             return false;
//     return true;
// }
