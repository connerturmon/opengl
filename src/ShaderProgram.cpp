#include "ShaderProgram.h"

#include <iostream>
#include <glad/glad.h>

ShaderProgram::ShaderProgram()
{
	program_id = glCreateProgram();
	memset(error_log, '\0', ERROR_LOG_SIZE);
}