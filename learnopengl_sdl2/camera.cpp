#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void camera_init(Camera* camera, f32 x, f32 y, f32 z, const i32 screen_width, const i32 screen_height) {
    camera->pos = glm::vec3(x, y, z);
    camera->target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera->direction = glm::normalize(camera->pos - camera->target);
    camera->_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera->right = glm::normalize(glm::cross(camera->_up, camera->direction));
    camera->up = glm::cross(camera->direction, camera->right);
    camera->front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera->zoom = 45.0f;
    camera->yaw = -90.0f;
    camera->width = screen_width;
    camera->height = screen_height;
}

void camera_project(Camera* camera, f32 near, f32 far) {
    camera->projection = glm::perspective(glm::radians(camera->zoom), (f32)camera->width / (f32)camera->height, near, far);
}

glm::mat4 camera_get_view(Camera* camera) {
    return glm::lookAt(camera->pos, camera->pos + camera->front, camera->up);
}

void camera_move_forward(Camera* camera, f32 amount) {
    camera->pos += amount * camera->front;
}

void camera_move_backward(Camera* camera, f32 amount) {
    camera->pos -= amount * camera->front;
}

void camera_move_left(Camera* camera, f32 amount) {
    camera->pos -= glm::normalize(glm::cross(camera->front, camera->up)) * amount;
}

void camera_move_right(Camera* camera, f32 amount) {
    camera->pos += glm::normalize(glm::cross(camera->front, camera->up)) * amount;
}

void camera_move(Camera* camera, f32 x, f32 y) {
    camera->yaw += x;
    camera->pitch += y;

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    direction.y = sin(glm::radians(camera->pitch));
    direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->front = glm::normalize(direction);
}

void camera_scroll(Camera* camera, f32 amount) {
    camera->zoom -= amount;
    if (camera->zoom < 1.0f) camera->zoom = 1.0f;
    if (camera->zoom > 45.0f) camera->zoom = 45.0f;
}