#include "renderer.h"
#include <SDL2/SDL_image.h>

#ifdef _DEBUG
#include <iostream>
#endif

#include <glm/gtc/matrix_transform.hpp>

void renderer_init(Renderer* renderer, u32 texture_count) {
	glGenVertexArrays(1, &renderer->VAO);
	glGenBuffers(2, renderer->buffers);
	if (texture_count != 0) {
		glGenTextures(texture_count, renderer->textures);
		renderer->texture_count = texture_count;
	}
	renderer->textures_created = 0;
	renderer->attribs_created = 0;
	renderer->attributes_stride = 0;
}

void renderer_begin(Renderer* renderer) {
	glBindVertexArray(renderer->VAO);
}

void renderer_bind_vbo(Renderer* renderer, f32* vertices, u32 vertices_size) {
	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
}

void renderer_bind_ebo(Renderer* renderer, u32* elements, u32 elements_size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size, elements, GL_STATIC_DRAW);
	renderer->has_ebo = true;
}

void renderer_bind_texture(Renderer* renderer, const char* path, u32 format) {
	if (renderer->textures_created >= renderer->texture_count)
	{
#ifdef _DEBUG
		std::cout << "Run out of active textures!" << std::endl;
#endif
		return;
	}

	SDL_Surface* surface = IMG_Load(path);

	glActiveTexture(GL_TEXTURE0 + renderer->textures_created);
	glBindTexture(GL_TEXTURE_2D, renderer->textures[renderer->textures_created]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surface);

	renderer->textures_created++;
}

void renderer_bind_attributes(Renderer* renderer, u32* attributes, u32 attributes_length) {
	u32 total_stride = 0;
	for (u32 i = 0; i < attributes_length; i++) {
		total_stride += attributes[i];
	}
	u32 current_stride = 0;
	for (u32 i = 0; i < attributes_length; i++) {
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, total_stride * sizeof(f32), (void*)(current_stride * sizeof(f32)));
		glEnableVertexAttribArray(i);
		current_stride += attributes[i];
	}
}

void renderer_end(Renderer* renderer) {
#ifdef _DEBUG
	if (renderer->textures_created != renderer->texture_count) std::cout << "WARNING: did not assign all created textures." << std::endl;
#endif
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void clear(f32 r, f32 g, f32 b, f32 a) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void renderer_draw(Renderer* renderer, i32 count) {
	for (u32 i = 0; i < renderer->texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderer->textures[i]);
	}
	glBindVertexArray(renderer->VAO);
	if (renderer->has_ebo) glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, count);
}

void renderer_draw(Renderer* renderer, glm::vec3* cube_positions, u32 cube_position_length, Shader* shader, i32 count) {
	glBindVertexArray(renderer->VAO);
	for (u32 i = 0; i < cube_position_length; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cube_positions[i]);
		f32 angle = 20.0f * (f32)i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader_set(shader, "model", model);
		glDrawArrays(GL_TRIANGLES, 0, count);
	}
}