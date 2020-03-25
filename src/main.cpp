#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Cube.h"
#include "Texture.h"
#include "Camera.h"

const unsigned int WIN_WIDTH = 1920;
const unsigned int WIN_HEIGHT = 1080;

void glfwErrorCallback(int code, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = WIN_WIDTH / 2, last_y = WIN_HEIGHT / 2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
		glfwGetPrimaryMonitor(), NULL
		);
	glfwMakeContextCurrent(main_window);
	glfwSetCursorPosCallback(main_window, mouseCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(main_window, framebufferSizeCallback);

	glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Cube cube;
	cube.bind();

	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint light_vao;
	glGenVertexArrays(1, &light_vao);
	glBindVertexArray(light_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SHADERS AND SHIT
	Shader vertex_shader("./src/shaders/vertex.vert", GL_VERTEX_SHADER);
	Shader lighting_shader("./src/shaders/lighting.frag", GL_FRAGMENT_SHADER);
	if (!vertex_shader.compile())
		std::cout << "FAILED TO COMPILE VERTEX SHADER:\n"
		<< vertex_shader.errorLog() << std::endl;
	if (!lighting_shader.compile())
		std::cout << "FAILED TO COMPILE FRAGMENT SHADER:\n"
		<< lighting_shader.errorLog() << std::endl;
	
	Shader light_shader("./src/shaders/light.frag", GL_FRAGMENT_SHADER);
	if (!light_shader.compile())
		std::cout << "FAILED TO COMPILE LIGHT SHADER:\n"
		<< lighting_shader.errorLog() << std::endl;

	ShaderProgram lighting_program;
	lighting_program.attachShader(vertex_shader.getid());
	lighting_program.attachShader(lighting_shader.getid());
	if (!lighting_program.link())
		std::cout << "FAILED TO LINK SHADERS:\n"
		<< lighting_program.errorLog() << std::endl;
	
	ShaderProgram light_program;
	light_program.attachShader(vertex_shader.getid());
	light_program.attachShader(light_shader.getid());
	if (!light_program.link())
		std::cout << "FAILED TO LINK LIGHT SHADERS:\n"
		<< lighting_program.errorLog() << std::endl;

	bool translated = false;
	// MAIN LOOP
	while (!glfwWindowShouldClose(main_window))
	{
		std::cout << "\nNew frame" << std::endl;
		glClearColor(0.02f, 0.10f, 0.12f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(main_window);

		glm::vec3 light_pos = glm::vec3(2.0f, 1.0f, -0.5f);
		glm::vec3 cube_color = glm::vec3(0.8f, 0.1f, 0.0f);
		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

		lighting_program.use();

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.viewMatrix();
		lighting_program.uniformMatrix("view", 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(80.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		lighting_program.uniformMatrix("projection", 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model = glm::mat4(1.0f);

		lighting_program.uniform3f("cube_color", cube_color);
		lighting_program.uniform3f("light_color", light_color);
		lighting_program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
		lighting_program.uniform3f("light_pos", light_pos);
		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		light_program.use();
		light_program.uniformMatrix("projection", 1, GL_FALSE, glm::value_ptr(projection));
		light_program.uniformMatrix("view", 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.0f);
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		light_program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(light_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

void processInput(GLFWwindow *window)
{
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processInput(CameraDirection::FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processInput(CameraDirection::BACK, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processInput(CameraDirection::LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processInput(CameraDirection::RIGHT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processInput(CameraDirection::DOWN, delta_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processInput(CameraDirection::UP, delta_time);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	camera.processMouseInput(xoffset, yoffset);
}
