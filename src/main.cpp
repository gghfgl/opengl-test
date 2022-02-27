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

    Plateform_Memory gameMemory = {};
    gameMemory.permanentStorageSize = MB(64);
    gameMemory.transientStorageSize = MB(512);
    //gameMemory.transientStorageSize = GB((uint64)4);
    AllocateMemory(&gameMemory);

    ASSERT(sizeof(Game) <= gameMemory.permanentStorageSize);
    Game* demo = (Game*)gameMemory.permanentStorage;
    InitGameAndLoadAssets(demo, window->width, window->height);
    SpriteRenderer renderer = InitSpriteRenderer();

    while (demo->state != GAME_EXIT) {
        // update delta time
        // -----------------
        UpdateFrameTime(time);

        // manage user input
        // -----------------
        PollEvents();
        ProcessInput(demo, keyboard, mouse, time->deltaTime);

        // update state and render
        // ------
        ClearBuffer();
        UpdateAndRender(demo, &renderer);
        SwapBuffer(window);
    }

    ClearSpriteRenderer(&renderer);
    TerminateWindow(window);
    delete(time);
    delete(keyboard);
    delete(mouse);
    delete(window);

    return 0;
}
