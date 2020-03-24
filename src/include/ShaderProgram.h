#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>

class ShaderProgram : public ShaderBase
{
public:
	ShaderProgram();

	void attachShader(GLuint shader) { glAttachShader(id, shader); }
	void use() const { glUseProgram(id); }
	void uniform4f(
		const char *uniform,
		GLfloat x,
		GLfloat y,
		GLfloat z,
		GLfloat w);
	bool link();

private:
	virtual bool checkError();
};