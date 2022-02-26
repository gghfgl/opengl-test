#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "internal.h"
#include "keyboard.h"
#include "plateform.h"
#include "shader.h"
#include "texture.h"
#include "game.h"

// GLFW function declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam );

const unsigned int SCREEN_WIDTH = 1900;
const unsigned int SCREEN_HEIGHT = 1440;

Game Breakout;;

int main(int argc, char *argv[])
{
    Window* window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CrapEngine 2D");
    KeyboardEvent* keyboard = InitKeyboardEvent();
    MouseEvent* mouse = InitMouseEvent(SCREEN_WIDTH, SCREEN_HEIGHT);
    FrameTime* time = InitFrameTime();
    // renderer ...
    
    // initialize game
    // ---------------
    Breakout = InitGame(window->width, window->height);

    while (!glfwWindowShouldClose(window->context)) // @delete: Replace with correct statement.
    {
        // update delta time
        // -----------------
        UpdateFrameTime(time);

        // poll events
        // -----------------
        PollEvents();
        if (keyboard->isPressed[keyboard::CRAP_KEY_ESCAPE])
            glfwSetWindowShouldClose(window->context, true);

        // manage user input
        // -----------------
        ProcessInput(&Breakout, time->deltaTime);

        // update game state
        // -----------------
        Update(&Breakout, time->deltaTime);

        // render
        // ------
        ClearBuffer();
        Render(&Breakout, Breakout.SpriteRenderer); // @improve: Meh ...
        SwapBuffer(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    TerminateWindow(window);
    return 0;
}
