#include "shader.h"

#ifdef _DEBUG
#include <iostream>
#endif

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

void shader_init(Shader* shader, const char* vertex, const char* fragment) {
	size_t vertex_file_size;
	void* vertex_file = SDL_LoadFile(vertex, &vertex_file_size);
#ifdef _DEBUG
	if (!vertex_file) {
		std::cout << "Failed to open file " << vertex << std::endl;
		return;
	}
#endif

	size_t fragment_file_size;
	void* fragment_file = SDL_LoadFile(fragment, &fragment_file_size);
#ifdef _DEBUG
	if (!fragment_file) {
		std::cout << "Failed to open file " << fragment << std::endl;
		return;
	}
#endif

#ifdef _DEBUG
	int success;
	char info_log[512];
#endif

	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const char* const*)&vertex_file, nullptr);
	glCompileShader(vertex_shader);

#ifdef _DEBUG
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
		return;
	}
#endif

	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const char* const*)&fragment_file, nullptr);
	glCompileShader(fragment_shader);

#ifdef _DEBUG
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
		return;
	}
#endif

	shader->id = glCreateProgram();
	glAttachShader(shader->id, vertex_shader);
	glAttachShader(shader->id, fragment_shader);
	glLinkProgram(shader->id);

#ifdef _DEBUG
	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader->id, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
		return;
	}
#endif

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	SDL_free(fragment_file);
	SDL_free(vertex_file);
}

void shader_use(Shader* shader) {
	glUseProgram(shader->id);
}

void shader_set(Shader* shader, const char* name, f32 x, f32 y, f32 z, f32 w) {
	i32 location = glGetUniformLocation(shader->id, name);
	glUniform4f(location, x, y, z, w);
}

void shader_set(Shader* shader, const char* name, i32 i) {
	i32 location = glGetUniformLocation(shader->id, name);
	glUniform1i(location, i);
}

void shader_set(Shader* shader, const char* name, glm::mat4 matrix) {
	u32 location = glGetUniformLocation(shader->id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}