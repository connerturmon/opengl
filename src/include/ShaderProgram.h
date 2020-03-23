#pragma once
#include "ShaderBase.h"

#include <glad/glad.h>

class ShaderProgram : public ShaderBase
{
public:
	ShaderProgram();

	void attachShader(GLuint shader) { glAttachShader(id, shader); }
	void use() const { glUseProgram(id); }
	bool link();

private:
	virtual bool checkError();
};