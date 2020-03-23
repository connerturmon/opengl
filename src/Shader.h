#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
	Shader(const char* path, GLenum type);
	~Shader() { glDeleteShader(shader); }

	const char* getPath() const { return path; }
	const char* getSource() const { return source.c_str(); }
	const char* errorLog() const { return error_log; }
	GLuint getShader() const { return shader; }
	GLenum getType() const { return type; }

	bool compile();

private:
	enum {
		ERROR_LOG_SIZE = 512
	};

	const char* path;
	std::string source;
	GLenum type;
	GLuint shader;
	char error_log[ERROR_LOG_SIZE];
};