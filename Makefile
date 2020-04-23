# Must be compiled before C-Source
SOURCE = src/*.cpp
IMGUI_SOURCE = dependencies/imgui/*.cpp

# Must be compiled after SOURCE
C_SOURCE = -x c dependencies/glad/glad.c

INC = -Isrc/include/ -Idependencies/

LFLAGS_MACOS = -lglfw
LFLAGS_LINUX = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

macos:
	clang++ $(INC) $(SOURCE) $(IMGUI_SOURCE) $(C_SOURCE) $(LFLAGS_MACOS)

linux:
	clang++ $(INC) $(SOURCE) $(IMGUI_SOURCE) $(C_SOURCE) $(LFLAGS_LINUX)