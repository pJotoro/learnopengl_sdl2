#include "app.h"
#include <glad/glad.h>

void app_init(App* app, int width, int height, const char* title, AppUpdateProc update, AppInputProc key_pressed, AppInputProc key_down, AppInputProc key_released, AppMouseMovedProc mouse_moved, AppMouseWheelProc mouse_wheel) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetSwapInterval(-1);

	app->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_MOUSE_GRABBED);

	app->gl_context = SDL_GL_CreateContext(app->window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) return;

	glViewport(0, 0, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	float border_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_DEPTH_TEST);

	app->update = update;
	app->key_down = key_down;
	app->key_pressed = key_pressed;
	app->key_released = key_released;
	app->mouse_moved = mouse_moved;
	app->mouse_wheel = mouse_wheel;
}

void app_run(App* app) {
	app->delta = 0.6f;

	for (;;) {
		f32 current_frame = (f32)SDL_GetTicks();
		app->delta = current_frame - app->last_frame;
		app->last_frame = current_frame;

		bool running = true;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
					break;
				}
				else {
					if ((event.key.keysym.sym == SDLK_w) && !app->up) {
						app->key_pressed(Key::UP, app->delta);
						app->up = true;
					}
					else if ((event.key.keysym.sym == SDLK_s) && !app->down) {
						app->key_pressed(Key::DOWN, app->delta);
						app->down = true;
					}
					else if ((event.key.keysym.sym == SDLK_a) && !app->left) {
						app->key_pressed(Key::LEFT, app->delta);
						app->left = true;
					}
					else if ((event.key.keysym.sym == SDLK_d) && !app->right) {
						app->key_pressed(Key::RIGHT, app->delta);
						app->right = true;
					}
				}
			} else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_w) {
					app->key_released(Key::UP, app->delta);
					app->up = false;
				} else if (event.key.keysym.sym == SDLK_s) {
					app->key_released(Key::DOWN, app->delta);
					app->down = false;
				} else if (event.key.keysym.sym == SDLK_a) {
					app->key_released(Key::LEFT, app->delta);
					app->left = false;
				} else if (event.key.keysym.sym == SDLK_d) {
					app->key_released(Key::RIGHT, app->delta);
					app->right = false;
				}
			} else if (event.type == SDL_MOUSEMOTION) {
				app->mouse_moved(event.motion.xrel, -event.motion.yrel);
			} else if (event.type == SDL_MOUSEWHEEL) {
				app->mouse_wheel(event.wheel.y);
			}
		}
		if (!running) break;

		if (app->left) app->key_down(Key::LEFT, app->delta);
		if (app->right) app->key_down(Key::RIGHT, app->delta);
		if (app->up) app->key_down(Key::UP, app->delta);
		if (app->down) app->key_down(Key::DOWN, app->delta);

		app->update(app->delta);

		SDL_GL_SwapWindow(app->window);

		current_frame = (f32)SDL_GetTicks();
		if (current_frame - app->last_frame < (1000.0f / 144.0f)) SDL_Delay((1000.0f / 144.0f) - (current_frame - app->last_frame));
	}
}