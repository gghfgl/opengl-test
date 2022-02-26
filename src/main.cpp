#include <iostream>

#include <glad/glad.h> // Remove with glfwWindowShouldClose().
#include <GLFW/glfw3.h>

#include "internal.h"
#include "plateform.h"
#include "game.h"

const unsigned int SCREEN_WIDTH = 1900;
const unsigned int SCREEN_HEIGHT = 1440;

int main(int argc, char *argv[]) {
    Window* window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CrapEngine 2D");
    KeyboardEvent* keyboard = InitKeyboardEvent();
    MouseEvent* mouse = InitMouseEvent(SCREEN_WIDTH, SCREEN_HEIGHT);
    FrameTime* time = InitFrameTime();
    SpriteRenderer* renderer = InitSpriteRenderer();

    // Memory arena for game assets.
    Game* demo = InitGame(window->width, window->height);

    while (demo->state != GAME_EXIT) {
        // update delta time
        // -----------------
        UpdateFrameTime(time);

        // manage user input
        // -----------------
        PollEvents();
        ProcessInput(demo, keyboard, mouse, time->deltaTime);

        // update game state
        // -----------------
        Update(demo, time->deltaTime);

        // render
        // ------
        ClearBuffer();
        RenderGame(demo, renderer);
        SwapBuffer(window);
    }

    delete(demo); // Clean memory arena ?
    ClearSpriteRenderer(renderer);
    delete(renderer); // Clear VAO renderer?
    delete(time);
    delete(keyboard);
    delete(mouse);
    TerminateWindow(window);
    delete(window);

    return 0;
}
