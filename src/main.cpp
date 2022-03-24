#include "win32.h"
#include "opengl.h"
#include "camera.h"
#include "game.h"

const unsigned int SCREEN_WIDTH = 1900;
const unsigned int SCREEN_HEIGHT = 1440;

int main(int argc, char *argv[]) {
    Plateform_Window* window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CrapEngine 2D");
    Plateform_KeyboardEvent* keyboard = InitKeyboardEvent();
    Plateform_MouseEvent* mouse = InitMouseEvent(SCREEN_WIDTH, SCREEN_HEIGHT);
    Plateform_FrameTime* time = InitFrameTime();
    
    SpriteRenderer renderer = InitSpriteRenderer();
    Camera camera = InitCamera((float32)window->width,
                               (float32)window->height,
                               glm::vec2(0.0f, 0.0f));
    Game demo = InitGameAndLoadAssets(window->width, window->height);

    while (demo.state != GAME_EXIT) {
        // update delta time
        // -----------------
        UpdateFrameTime(time);

        // manage user input
        // -----------------
        PollEvents();
        ProcessInput(&demo, &camera, keyboard, mouse, time->deltaTime);

        // update state and render
        // ------
        ClearBuffer();
        UpdateAndRender(&demo, &camera, &renderer, mouse);
        SwapBuffer(window);

        // memory tracker
        // ------
#if DEBUG
        //PrintMemoryTrackerUsage(MEM_TRACK);
#endif
    }

    // Clear textures?
    ClearSpriteRenderer(&renderer);
    TerminateWindow(window);
    delete(time);
    delete(keyboard);
    delete(mouse);
    delete(window);

    return 0;
}
