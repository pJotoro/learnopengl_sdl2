#pragma once

#include <SDL2/SDL.h>
#include "types.h"

enum class Key {
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

typedef void (*AppUpdateProc)(f32);
typedef void (*AppInputProc)(Key, f32);
typedef void (*AppMouseMovedProc)(i32, i32);
typedef void (*AppMouseWheelProc)(i32);

struct App {
	SDL_Window* window;
	SDL_GLContext gl_context;
	AppUpdateProc update;
	AppInputProc key_down;
	AppInputProc key_pressed;
	AppInputProc key_released;
	AppMouseMovedProc mouse_moved;
	AppMouseWheelProc mouse_wheel;
	f32 delta, last_frame;
	
	bool left, right, up, down;
};

void app_init(App* app, int width, int height, const char* title, AppUpdateProc update, AppInputProc key_pressed, AppInputProc key_down, AppInputProc key_released, AppMouseMovedProc mouse_moved, AppMouseWheelProc mouse_wheel);
void app_run(App* app);