#include "win32.h"
#include "opengl.h"
#include "game.h"

const unsigned int SCREEN_WIDTH = 1900;
const unsigned int SCREEN_HEIGHT = 1440;

int main(int argc, char *argv[]) {
    Plateform_Window* window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CrapEngine 2D");
    Plateform_KeyboardEvent* keyboard = InitKeyboardEvent();
    Plateform_MouseEvent* mouse = InitMouseEvent(SCREEN_WIDTH, SCREEN_HEIGHT);
    Plateform_FrameTime* time = InitFrameTime();
    
    SpriteRenderer renderer = InitSpriteRenderer();
    Game demo = InitGameAndLoadAssets(window->width, window->height);

    while (demo.state != GAME_EXIT) {
        // update delta time
        // -----------------
        UpdateFrameTime(time);

        // manage user input
        // -----------------
        PollEvents();
        ProcessInput(&demo, keyboard, mouse, time->deltaTime);

        // update state and render
        // ------
        ClearBuffer();
        UpdateAndRender(&demo, &renderer);
        SwapBuffer(window);

        // memory tracker
        // ------
#if DEBUG
        //PrintMemoryTrackerUsage(MEM_TRACK);
#endif
    }

    ClearSpriteRenderer(&renderer);
    TerminateWindow(window);
    delete(time);
    delete(keyboard);
    delete(mouse);
    delete(window);

    return 0;
}
