#pragma once


enum CameraDirection {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
};

struct Camera {
    float32 speed;
    glm::vec2 position;
    float32 zoom;

    float32 width;
    float32 height;
};

void updateCameraViewMatrix(Camera* camera);

Camera InitCamera(float32 width, float32 height, glm::vec2 position) {
    Camera camera;
    camera.position = position;
    camera.speed = 50.0f;
    camera.zoom = 1.0f;

    camera.width = width;
    camera.height = height;

    return camera;
}
