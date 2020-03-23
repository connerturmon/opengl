#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

Shader::Shader(const char *path, GLenum type)
	: path(path), type(type)
{
	memset(error_log, '\0', ERROR_LOG_SIZE);

	std::ifstream shader_file;

	if (!shader_file.is_open())
	{
		// Ensure exceptions can be thrown
		shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shader_file.open(path);
			std::stringstream content_buffer;
			content_buffer << shader_file.rdbuf();
			source = content_buffer.str();
			shader_file.close();
		}
		catch (std::ifstream::failure exception)
		{
			std::cerr << "ERROR: Failed to read shader file."
				<< std::endl;
		}
	}

	shader = glCreateShader(type);
	const char* shader_source = source.c_str();
	glShaderSource(shader, 1, &shader_source, NULL);
}

bool Shader::compile()
{
	glCompileShader(shader);

	// If shader fails to compile, the method will return false
	// and the error_log will be populated. The use can then use the
	// Shader::errorLog() method to read the error.
	int compilation_success;
	bool success = true;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_success);
	if (!compilation_success)
	{
		glGetShaderInfoLog(shader, ERROR_LOG_SIZE, NULL, error_log);
		success = false;
	}

	return success;
}