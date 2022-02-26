#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void GLAPIENTRY debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyboard_callback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mode);
void mouse_button_callback(GLFWwindow *window, int32 button, int32 action, int32 mods);
void cursor_position_callback(GLFWwindow *window, float64 xpos, float64 ypos);
void mouse_scroll_callback(GLFWwindow *window, float64 xoffset, float64 yoffset);

struct Window {
    uint32 width, height;
    const char* title;
    GLFWwindow *context;
};

Window* InitWindow(uint32 width, uint32 height, const char *title) {
    // glfw ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *glWindow = glfwCreateWindow((int)width, (int)height, title, nullptr, nullptr);
    glfwMakeContextCurrent(glWindow);
    // glfwSwapInterval(1); // Vsync

    glfwSetKeyCallback(glWindow, keyboard_callback);
    glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);
    glfwSetKeyCallback(glWindow, keyboard_callback);
    glfwSetMouseButtonCallback(glWindow, mouse_button_callback);
    glfwSetCursorPosCallback(glWindow, cursor_position_callback);
    glfwSetScrollCallback(glWindow, mouse_scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwGetWindowSize(window, &width, &height);

    // glad ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Log::error("WINDOW: Failed to initialize GLAD");        
        exit(666);
    }

    // opengl ---
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debug_message_callback, 0);

    // window ---
    Window* window = new Window;
    if (!window) {
        Log::error("PLATEFORM: error wile allocating Window memory");
    }
    window->width = width;
    window->height = height;
    window->title = title;
    window->context = glWindow;

    return window;
}

void ClearBuffer() {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SwapBuffer(Window* window) {
    glfwSwapBuffers(window->context);
}

void PollEvents() {
    glfwPollEvents();
}

void TerminateWindow(Window* window) {
    glfwDestroyWindow(window->context);
    glfwTerminate();
}

struct FrameTime {
    float32 deltaTime;
    float32 lastFrame;
};

FrameTime* InitFrameTime() {
    FrameTime* time = new FrameTime;
    if (!time) {
        Log::error("PLATEFORM: error wile allocating FrameTime memory");
    }
    time->deltaTime = 0.0;
    time->lastFrame = 0.0;

    return time;
}

void UpdateFrameTime(FrameTime* time) {
    float32 currentFrame = (float32)glfwGetTime();
    time->deltaTime = currentFrame - time->lastFrame;
    time->lastFrame = currentFrame;
}

struct KeyboardEvent {
    bool isPressed[512] = {0};
    bool isReleased[512] = {0};
};

KeyboardEvent *PLATEFORM_KEYBOARD_EVENT;
KeyboardEvent* InitKeyboardEvent() {
    PLATEFORM_KEYBOARD_EVENT = new KeyboardEvent;
    if (!PLATEFORM_KEYBOARD_EVENT) {
        Log::error("PLATEFORM: error wile allocating PLATEFORM_KEYBOARD_EVENT memory");
    }
    return PLATEFORM_KEYBOARD_EVENT;
}

struct MouseEvent {
    float64 posX, posY;
    float64 lastX, lastY;
    float32 offsetX, offsetY;
    float64 scrollOffsetX, scrollOffsetY;
    bool leftButton;
    bool leftButtonFirstClick;
    bool rightButton;
    bool rightButtonFirstClick;
};

MouseEvent *PLATEFORM_MOUSE_EVENT;
MouseEvent* InitMouseEvent(uint32 width, uint32 height) {
    PLATEFORM_MOUSE_EVENT = new MouseEvent;
    if (!PLATEFORM_MOUSE_EVENT) {
        Log::error("PLATEFORM: error wile allocating PLATEFORM_MOUSE_EVENT memory");
    }
    PLATEFORM_MOUSE_EVENT->posX = 0.0f;
    PLATEFORM_MOUSE_EVENT->posY = 0.0f;
    PLATEFORM_MOUSE_EVENT->offsetX = 0.0f;
    PLATEFORM_MOUSE_EVENT->offsetY = 0.0f;
    PLATEFORM_MOUSE_EVENT->scrollOffsetX = 0.0f;
    PLATEFORM_MOUSE_EVENT->scrollOffsetY = 0.0f;
    PLATEFORM_MOUSE_EVENT->lastX = width / 2.0f;
    PLATEFORM_MOUSE_EVENT->lastY = height / 2.0f;
    PLATEFORM_MOUSE_EVENT->leftButton = false;
    PLATEFORM_MOUSE_EVENT->leftButtonFirstClick = true;
    PLATEFORM_MOUSE_EVENT->rightButton = false;
    PLATEFORM_MOUSE_EVENT->rightButtonFirstClick = true;

    return PLATEFORM_MOUSE_EVENT;
}

// @check: Is it usefull?
void UpdateMouseOffsets(MouseEvent* mouse) {
    if (mouse->leftButtonFirstClick) {
        mouse->lastX = mouse->posX;
        mouse->lastY = mouse->posY;
        mouse->leftButtonFirstClick = false;
    }

    mouse->offsetX = (float32)(mouse->posX - mouse->lastX);
    mouse->offsetY = (float32)(mouse->lastY - mouse->posY);

    mouse->lastX = mouse->posX;
    mouse->lastY = mouse->posY;
}

// @check: Is it usefull?
float32 GetMouseScrollOffsetY(MouseEvent* mouse) {
    float32 rValue = (float32)mouse->scrollOffsetY;
    mouse->scrollOffsetY = 0.0f;

    return rValue;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyboard_callback(GLFWwindow* window, int32 key, int32, int32 action, int32) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            PLATEFORM_KEYBOARD_EVENT->isPressed[key] = true;
            PLATEFORM_KEYBOARD_EVENT->isReleased[key] = false;
        } else if (action == GLFW_RELEASE) {
            PLATEFORM_KEYBOARD_EVENT->isPressed[key] = false;
            PLATEFORM_KEYBOARD_EVENT->isReleased[key] = true;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int32 button, int32 action, int32) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        PLATEFORM_MOUSE_EVENT->leftButton = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        PLATEFORM_MOUSE_EVENT->leftButton = false;
        PLATEFORM_MOUSE_EVENT->leftButtonFirstClick = true;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        PLATEFORM_MOUSE_EVENT->rightButton = true;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        PLATEFORM_MOUSE_EVENT->rightButton = false;
        PLATEFORM_MOUSE_EVENT->rightButtonFirstClick = true;
    }
}

void cursor_position_callback(GLFWwindow* window, float64 xpos, float64 ypos) {
    PLATEFORM_MOUSE_EVENT->posX = xpos;
    PLATEFORM_MOUSE_EVENT->posY = ypos;
}

void mouse_scroll_callback(GLFWwindow* window, float64 xoffset, float64 yoffset) {
    PLATEFORM_MOUSE_EVENT->scrollOffsetX = xoffset;
    PLATEFORM_MOUSE_EVENT->scrollOffsetY = yoffset;
}

void GLAPIENTRY debug_message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ) {
    const char *_source;
    const char *_type;
    const char *_severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }
        
    // fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    //          ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
    //           type, severity, message );

    fprintf( stderr, "GL CALLBACK: %s type = %s, severity = %s, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             _type, _severity, message );
}
