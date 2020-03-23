#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

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

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	// bottom left
		 0.5f, -0.5f, 0.0f,	// bottom right
		 0.0f,  0.5f, 0.0f	// top
	};

	unsigned int vbo;
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader vertex_shader("./vertex.vert", GL_VERTEX_SHADER);
	Shader fragment_shader("./fragment.frag", GL_FRAGMENT_SHADER);
	if (!vertex_shader.compile())
		std::cout << "FAILED TO COMPILE VERTEX SHADER:\n"
		<< vertex_shader.errorLog() << std::endl;
	if (!fragment_shader.compile())
		std::cout << "FAILED TO COMPILE FRAGMENT SHADER:\n"
		<< fragment_shader.errorLog() << std::endl;

	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader.getShader());
	glAttachShader(shader_program, fragment_shader.getShader());
	glLinkProgram(shader_program);
	int success;
	char error_log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, error_log);
		std::cout << "ERROR LINKING SHADERS: " << error_log << std::endl;
	}

	while (!glfwWindowShouldClose(main_window))
	{
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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