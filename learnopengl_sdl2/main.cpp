#include "app.h"
#include "shader.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

static Shader the_shader;
static Renderer renderer;
static Camera camera;

static glm::vec3 cube_positions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

void update_callback(f32 delta);
void key_pressed_callback(Key key, f32 delta);
void key_down_callback(Key key, f32 delta);
void key_released_callback(Key key, f32 delta);
void mouse_moved_callback(i32 xmotion, i32 ymotion);
void mouse_wheel_callback(i32 amount);

const i32 SCREEN_WIDTH = 3840;
const i32 SCREEN_HEIGHT = 2160;

int main(int argc, char** argv) {
	App app = {};
	app_init(&app, SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", update_callback, key_pressed_callback, key_down_callback, key_released_callback, mouse_moved_callback, mouse_wheel_callback);

	f32 vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//u32 indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};

	renderer_init(&renderer, 2);
	renderer_begin(&renderer);
	renderer_bind_vbo(&renderer, vertices, sizeof(vertices));
	//renderer_bind_ebo(&renderer, indices, sizeof(indices));
	renderer_bind_texture(&renderer, "C:/Users/jonas/Downloads/container.jpg", GL_RGB);
	renderer_bind_texture(&renderer, "C:/Users/jonas/Downloads/awesomeface.png");
	u32 attributes[] = { 3, 2 };
	renderer_bind_attributes(&renderer, attributes, sizeof(attributes) / sizeof(attributes[0]));
	renderer_end(&renderer);

	shader_init(&the_shader, "shader.vert", "shader.frag");
    shader_use(&the_shader);
    shader_set(&the_shader, "texture1", 0);
    shader_set(&the_shader, "texture2", 1);

    camera_init(&camera, 0.0f, 0.0f, 3.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

	app_run(&app);

	return 0;
}

void update_callback(f32 delta) {
	clear(0.2f, 0.3f, 0.3f, 1.0f);

    glm::mat4 view = camera_get_view(&camera);
   camera_project(&camera);

    shader_use(&the_shader);
    shader_set(&the_shader, "projection", camera.projection);
    shader_set(&the_shader, "view", view);

	renderer_draw(&renderer, cube_positions, sizeof(cube_positions) / sizeof(cube_positions[0]), &the_shader, 36);
}

void key_pressed_callback(Key key, f32 delta) {

}

void key_down_callback(Key key, f32 delta) {
    f32 camera_speed = 0.005f * delta;

    switch (key) {
    case Key::UP:
        camera_move_forward(&camera, camera_speed);
        break;
    case Key::DOWN:
        camera_move_backward(&camera, camera_speed);
        break;
    case Key::LEFT:
        camera_move_left(&camera, camera_speed);
        break;
    case Key::RIGHT:
        camera_move_right(&camera, camera_speed);
        break;
    }
}

void key_released_callback(Key key, f32 delta) {
	
}

void mouse_moved_callback(i32 xmotion, i32 ymotion) {
    f32 sensitivity = 0.1f;
    f32 xoffset = (f32)xmotion * sensitivity;
    f32 yoffset = (f32)ymotion * sensitivity;

    camera_move(&camera, xoffset, yoffset);
}

void mouse_wheel_callback(i32 amount) {
    camera_scroll(&camera, (f32)amount);
}