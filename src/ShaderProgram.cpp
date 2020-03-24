#include "ShaderProgram.h"

#include <iostream>
#include <cstring>

#include <glad/glad.h>

ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
	memset(error_log, '\0', ERROR_LOG_SIZE);
}

bool ShaderProgram::link()
{
	glLinkProgram(id);

	bool success = true;
	if (checkError() == false)
		success = false;
	return success;
}

bool ShaderProgram::checkError()
{
	int linkage_success;
	bool success = true;

	glGetProgramiv(id, GL_LINK_STATUS, &linkage_success);
	if (!linkage_success)
	{
		glGetProgramInfoLog(id, ERROR_LOG_SIZE, NULL, error_log);
		success = false;
	}

	return success;
}

void ShaderProgram::uniform4f(
	const char *uniform,
	GLfloat x,
	GLfloat y,
	GLfloat z,
	GLfloat w)
{
	int uniform_location = glGetUniformLocation(id, uniform);
	glUseProgram(id);
	glUniform4f(uniform_location, x, y, z, w);
}

void ShaderProgram::uniformMatrix(
	const char *uniform,
	GLsizei count,
	GLboolean transpose,
	const GLfloat *value)
{
	int uniform_location = glGetUniformLocation(id, uniform);
	glUniformMatrix4fv(uniform_location, count, transpose, value);
}