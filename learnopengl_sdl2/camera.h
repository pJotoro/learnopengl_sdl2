#include <glm/glm.hpp>
#include "types.h"

struct Camera {
    glm::mat4 projection;
    glm::vec3 pos;
    glm::vec3 target;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;
    glm::vec3 _up;
    f32 pitch;
    f32 yaw;
    f32 zoom;
    i32 width, height;
};

void camera_init(Camera* camera, f32 x, f32 y, f32 z, const i32 screen_width, const i32 screen_height);
void camera_project(Camera* camera, f32 near = 0.1f, f32 far = 100.0f);
glm::mat4 camera_get_view(Camera* camera);
void camera_move_forward(Camera* camera, f32 amount);
void camera_move_backward(Camera* camera, f32 amount);
void camera_move_left(Camera* camera, f32 amount);
void camera_move_right(Camera* camera, f32 amount);
void camera_move(Camera* camera, f32 x, f32 y);
void camera_scroll(Camera* camera, f32 amount);
