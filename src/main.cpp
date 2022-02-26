#include "win32.h"
#include "opengl.h"
#include "game.h"

const unsigned int SCREEN_WIDTH = 1900;
const unsigned int SCREEN_HEIGHT = 1440;

int main(int argc, char *argv[]) {
    PLATEFORM::Window* window = PLATEFORM::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CrapEngine 2D");
    PLATEFORM::KeyboardEvent* keyboard = PLATEFORM::InitKeyboardEvent();
    PLATEFORM::MouseEvent* mouse = PLATEFORM::InitMouseEvent(SCREEN_WIDTH, SCREEN_HEIGHT);
    PLATEFORM::FrameTime* time = PLATEFORM::InitFrameTime();

    SpriteRenderer* renderer = InitSpriteRenderer();
    Game* demo = InitGame(window->width, window->height); // @improve: Memory arena for game assets.

    while (demo->state != GAME_EXIT) {
        // update delta time
        // -----------------
        PLATEFORM::UpdateFrameTime(time);

        // manage user input
        // -----------------
        PLATEFORM::PollEvents();
        ProcessInput(demo, keyboard, mouse, time->deltaTime);

        // update state and render
        // ------
        PLATEFORM::ClearBuffer();
        UpdateAndRender(demo, renderer);
        PLATEFORM::SwapBuffer(window);
    }

    delete(demo); // Clean memory arena ?
    ClearSpriteRenderer(renderer);
    delete(renderer); // Clear VAO renderer?
    delete(time);
    delete(keyboard);
    delete(mouse);
    PLATEFORM::TerminateWindow(window);
    delete(window);

    return 0;
}
