#pragma once

#include <glad/glad.h>

class ShaderProgram
{
public:
	ShaderProgram();

	void attachShader(GLuint shader) { glAttachShader(program_id, shader); }
	void link() { glLinkProgram(program_id); }

private:
	enum {
		ERROR_LOG_SIZE
	};
	GLuint program_id;
	char error_log[ERROR_LOG_SIZE];
};