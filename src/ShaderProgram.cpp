#include "ShaderProgram.h"

#include <iostream>
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