#pragma once

#include "types.h"
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "shader.h"

struct Renderer {
	u32 VAO;
	union {
		u32 buffers[2];
		struct { u32 VBO; u32 EBO; };
	};
	u32 textures[16];
	u32 texture_count;
	u32 textures_created;
	u32 attribs_created;
	i32 attributes_stride;
	b32 has_ebo;
};

void renderer_init(Renderer* renderer, u32 texture_count = 0);
void renderer_begin(Renderer* renderer);
void renderer_bind_vbo(Renderer* renderer, f32* vertices, u32 vertices_size);
void renderer_bind_ebo(Renderer* renderer, u32* elements, u32 elements_size);
void renderer_bind_texture(Renderer* renderer, const char* path, u32 format = GL_RGBA);
void renderer_bind_attributes(Renderer* renderer, u32* attributes, u32 attributes_size);
void renderer_end(Renderer* renderer);

void clear(f32 r, f32 g, f32 b, f32 a);

void renderer_draw(Renderer* renderer, i32 count);
void renderer_draw(Renderer* renderer, glm::vec3* cube_positions, u32 cube_position_length, Shader* shader, i32 count);
