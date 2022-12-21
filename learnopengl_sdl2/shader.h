#pragma once

#include "types.h"
#include <glm/glm.hpp>

struct Shader {
	u32 id;
};

void shader_init(Shader* shader, const char* vertex, const char* fragment);
void shader_use(Shader* shader);
void shader_set(Shader* shader, const char* name, f32 x, f32 y, f32 z, f32 w);
void shader_set(Shader* shader, const char* name, i32 i);
void shader_set(Shader* shader, const char* name, glm::mat4 matrix);