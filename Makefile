# Must be compiled before C-Source
SOURCE = src/main.cpp src/Shader.cpp src/ShaderProgram.cpp

# Must be compiled after SOURCE
C_SOURCE = -x c src/glad.c

LFLAGS_MACOS = -lglfw

macos:
	clang++ $(SOURCE) $(C_SOURCE) $(LFLAGS_MACOS)