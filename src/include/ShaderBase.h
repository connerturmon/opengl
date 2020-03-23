#pragma once

#include <glad/glad.h>

class ShaderBase
{
public:
    GLuint getid() const { return id; }
    const char *errorLog() const { return error_log; };

protected:
	/* If shader fails to compile, the method will return false
	   and the error_log will be populated. The use can then use the
	   Shader::errorLog() method to read the error. */
    virtual bool checkError() = 0;

    enum {
        ERROR_LOG_SIZE = 512
    };
    GLuint id;
    char error_log[ERROR_LOG_SIZE];
};