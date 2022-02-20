#ifndef GAME_H
#define GAME_H

//static std::map<std::string, Shader>    ShaderCache;
//static std::map<std::string, Texture2D> TextureCache;

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

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
        // ...
    }

    void ProcessInput(float dt) {
        // ...
    }

    void Update(float dt) {
        // ...
    }

    void Render() {
        // ...
    }
};

#endif
