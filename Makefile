# Must be compiled before C-Source
SOURCE = src/main.cpp src/Shader.cpp src/ShaderProgram.cpp src/Cube.cpp

# Must be compiled after SOURCE
C_SOURCE = -x c dependencies/glad.c

INC = -Isrc/include/ -Idependencies/

LFLAGS_MACOS = -lglfw
LFLAGS_LINUX = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

macos:
	clang++ $(INC) $(SOURCE) $(C_SOURCE) $(LFLAGS_MACOS)

linux:
	clang++ $(INC) $(SOURCE) $(C_SOURCE) $(LFLAGS_LINUX)