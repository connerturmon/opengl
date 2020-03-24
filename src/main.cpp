#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"
#include "ShaderProgram.h"

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

void glfwErrorCallback(int code, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(int argc, const char* argv[])
{
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWwindow* main_window = glfwCreateWindow(
		WIN_WIDTH, WIN_HEIGHT,
		"Learning OpenGL",
		NULL, NULL
		);
	glfwMakeContextCurrent(main_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(main_window, framebufferSizeCallback);

	// CORDS AND SHIT
	GLfloat vertices[] = {
		// Vertices			// Texture Coords
		-0.5, -0.5, 0.0,	0.0, 0.0, 0.0,		// 0 Bottom Left
		-0.5,  0.5, 0.0,	0.0, 1.0, 0.0,		// 1 Top Left
		 0.5, -0.5, 0.0,	1.0, 0.0, 0.0,		// 2 Bottom Right
		 0.5,  0.5, 0.0,	1.0, 1.0, 0.0		// 3 Top Right
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	// BUFFERS AND SHIT
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// TEXTURE
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	int width, height, n_channels;
	unsigned char *data = stbi_load(
		"resource/container.jpg",
		&width,
		&height,
		&n_channels,
		0
	);
	if (data)
	{
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB,
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);

	// SHADERS AND SHIT
	Shader vertex_shader("./src/shaders/vertex.vert", GL_VERTEX_SHADER);
	Shader fragment_shader("./src/shaders/fragment.frag", GL_FRAGMENT_SHADER);
	if (!vertex_shader.compile())
		std::cout << "FAILED TO COMPILE VERTEX SHADER:\n"
		<< vertex_shader.errorLog() << std::endl;
	if (!fragment_shader.compile())
		std::cout << "FAILED TO COMPILE FRAGMENT SHADER:\n"
		<< fragment_shader.errorLog() << std::endl;

	ShaderProgram program;
	program.attachShader(vertex_shader.getid());
	program.attachShader(fragment_shader.getid());
	if (!program.link())
		std::cout << "FAILED TO LINK SHADERS:\n"
		<< program.errorLog() << std::endl;

	// MAIN LOOP
	while (!glfwWindowShouldClose(main_window))
	{
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.use();
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBindVertexArray(vao);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(main_window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void glfwErrorCallback(int code, const char* description)
{
	std::cout << "GLFW Error " << code << ": " << description
		<< std::endl;
	glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}