#pragma once
#include "ShaderBase.h"

#include <iostream>
#include <glad/glad.h>

class ShaderProgram : public ShaderBase
{
public:
	ShaderProgram();

	void attachShader(GLuint shader) { glAttachShader(id, shader); }
	void use() const { glUseProgram(id); std::cout << "Using program " << id << std::endl; }
	void uniform4f(
		const char *uniform,
		GLfloat x,
		GLfloat y,
		GLfloat z,
		GLfloat w
	);
	void uniformMatrix(
		const char *uniform,
		GLsizei count,
		GLboolean transpose,
		const GLfloat *value
	);
	void uniform3f(
		const char *uniform,
		GLfloat x,
		GLfloat y,
		GLfloat z
	);
	bool link();

private:
	virtual bool checkError();
};