#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void GLAPIENTRY debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyboard_callback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mode);
void mouse_button_callback(GLFWwindow *window, int32 button, int32 action, int32 mods);
void cursor_position_callback(GLFWwindow *window, float64 xpos, float64 ypos);
void mouse_scroll_callback(GLFWwindow *window, float64 xoffset, float64 yoffset);

constexpr uint32 PLATEFORM_RGB = GL_RGB;
constexpr uint32 PLATEFORM_RGBA = GL_RGBA;
constexpr uint32 PLATEFORM_REPEAT = GL_REPEAT;
constexpr uint32 PLATEFORM_LINEAR = GL_LINEAR;

// --------------------------------------------------------------
// ------------------------ OPENGL WINDOW -----------------------
// --------------------------------------------------------------
struct Plateform_Window {
    uint32 width, height;
    const char* title;
    GLFWwindow *context;
};

Plateform_Window* InitWindow(uint32 width, uint32 height, const char *title) {
    // glfw ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *glWindow = glfwCreateWindow((int)width, (int)height, title, nullptr, nullptr);
    glfwMakeContextCurrent(glWindow);
    glfwSwapInterval(1); // Vsync

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
    //glPolygonMode(GL_FRONT_AND_BACK, (GL_LINE));
    //glPolygonMode(GL_FRONT_AND_BACK, (GL_FILL));

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debug_message_callback, 0);

    // window ---
    Plateform_Window* window = new Plateform_Window;
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

void SwapBuffer(Plateform_Window* window) {
    glfwSwapBuffers(window->context);
}

void PollEvents() {
    glfwPollEvents();
}

void TerminateWindow(Plateform_Window* window) {
    glfwDestroyWindow(window->context);
    glfwTerminate();
}

// --------------------------------------------------------------
// ------------------------- OPENGL TIME ------------------------
// --------------------------------------------------------------
struct Plateform_FrameTime {
    float32 deltaTime;
    float32 lastFrame;
};

Plateform_FrameTime* InitFrameTime() {
    Plateform_FrameTime* time = new Plateform_FrameTime;
    if (!time) {
        Log::error("PLATEFORM: error wile allocating FrameTime memory");
    }
    time->deltaTime = 0.0;
    time->lastFrame = 0.0;

    return time;
}

void UpdateFrameTime(Plateform_FrameTime* time) {
    float32 currentFrame = (float32)glfwGetTime();
    time->deltaTime = currentFrame - time->lastFrame;
    time->lastFrame = currentFrame;
}

// --------------------------------------------------------------
// ------------------------ OPENGL INPUT ------------------------
// --------------------------------------------------------------
struct Plateform_KeyboardEvent {
    bool isPressed[512] = {0};
    bool isReleased[512] = {0};
};

Plateform_KeyboardEvent* PLATEFORM_KEYBOARD_EVENT;
Plateform_KeyboardEvent* InitKeyboardEvent() {
    PLATEFORM_KEYBOARD_EVENT = new Plateform_KeyboardEvent;
    if (!PLATEFORM_KEYBOARD_EVENT) {
        Log::error("PLATEFORM: error wile allocating PLATEFORM_KEYBOARD_EVENT memory");
    }
    return PLATEFORM_KEYBOARD_EVENT;
}

struct Plateform_MouseEvent {
    float64 posX, posY;
    float64 lastX, lastY;
    float32 offsetX, offsetY;
    float64 scrollOffsetX, scrollOffsetY;
    bool leftButton;
    bool leftButtonFirstClick;
    bool rightButton;
    bool rightButtonFirstClick;
};

Plateform_MouseEvent* PLATEFORM_MOUSE_EVENT;
Plateform_MouseEvent* InitMouseEvent(uint32 width, uint32 height) {
    PLATEFORM_MOUSE_EVENT = new Plateform_MouseEvent;
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

// @improve: Check left click here?
void UpdateMouseOffsets(Plateform_MouseEvent* mouse) {
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

float32 GetMouseScrollOffsetY(Plateform_MouseEvent* mouse) {
    float32 rValue = (float32)mouse->scrollOffsetY;
    mouse->scrollOffsetY = 0.0f;

    return rValue;
}


// --------------------------------------------------------------
// ------------------------ OPENGL SHADER -----------------------
// --------------------------------------------------------------
void SetShaderFloat(uint32 ID, const char *name, float32 value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void SetShaderInteger(uint32 ID, const char *name, int32 value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void SetShaderVector2f(uint32 ID, const char *name, float32 x, float32 y) {
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void SetShaderVector2f(uint32 ID, const char *name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}

void SetShaderVector3f(uint32 ID, const char *name, float32 x, float32 y, float32 z) {
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void SetShaderVector3f(uint32 ID, const char *name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void SetShaderVector4f(uint32 ID, const char *name, float32 x, float32 y, float32 z, float32 w) {
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}

void SetShaderVector4f(uint32 ID, const char *name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}

void SetShaderMatrix4(uint32 ID, const char *name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}

void UseShader(uint32 ID) {
    glUseProgram(ID);
}

void DeleteShader(uint32 ID) {
    glDeleteProgram(ID);
}

void check_shader_compile_errors(uint32 object, std::string type);
uint32 CompileShaderSources(const char* vertexSource, const char* fragmentSource) {
    uint32 vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    check_shader_compile_errors(vertexShader, "VERTEX");

    uint32 fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    check_shader_compile_errors(fragmentShader, "FRAGMENT");

    uint32 ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    check_shader_compile_errors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return ID;
}

void check_shader_compile_errors(uint32 object, std::string type) {
    int32 success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            Log::error("SHADER: Compile-time error: Type: %s\n %s\n", type.c_str(), infoLog);
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            Log::error("SHADER: Link-time error: Type: %s\n %s\n", type.c_str(), infoLog);
        }
    }
}

// --------------------------------------------------------------
// ---------------------- OPENGL TEXTURES -----------------------
// --------------------------------------------------------------
uint32 GenerateTexture(uint32 width, uint32 height, uint32 internalFormat, uint32 imageFormat, uint32 wrapAxisS, uint32 wrapAxisT, uint32 filterMin, uint32 filterMax, unsigned char* data) {
    uint32 ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapAxisS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapAxisT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);

    return ID;
}

void BindTexture(uint32 ID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void DeleteTexture(uint32* ID) {
    glDeleteTextures(1, ID);
}

// --------------------------------------------------------------
// ------------------------ OPENGL VAO --------------------------
// --------------------------------------------------------------
void DrawVertexArraysTriangles(uint32 VAO) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

uint32 BindVertexArraysFloat(float32 vertices[], uint32 size, uint32 padding) {
    uint32 VAO;
    glGenVertexArrays(1, &VAO);

    uint32 VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, padding, GL_FLOAT, GL_FALSE, padding * sizeof(float32), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void DeleteVertexArrays(uint32* VAO) {
    glDeleteVertexArrays(1, VAO);    
}

// --------------------------------------------------------------
// ---------------------- OPENGL CALLBACKS ----------------------
// --------------------------------------------------------------
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
