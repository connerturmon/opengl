#pragma once
#include "ShaderBase.h"

#include <string>
#include <glad/glad.h>

class Shader : public ShaderBase
{
public:
	Shader(const char *path, GLenum type);
	virtual ~Shader() { glDeleteShader(id); }

	const char *getPath() const { return path; }
	const char *getSource() const { return source.c_str(); }
	GLenum getType() const { return type; }
	bool compile();

private:
	virtual bool checkError();
	const char *path;
	std::string source;
	GLenum type;
};