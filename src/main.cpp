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

const unsigned int WIN_WIDTH = 1920;
const unsigned int WIN_HEIGHT = 1080;

void glfwErrorCallback(int code, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = WIN_WIDTH / 2, last_y = WIN_HEIGHT / 2;

float yaw = 0.0f, pitch = -90.0f;

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
	Texture texture("resource/container.jpg");

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

	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.4f, -1.5f, 1.0f),
		glm::vec3(3.0f, 0.0f, -4.0f)
	};

	// MAIN LOOP
	while (!glfwWindowShouldClose(main_window))
	{
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(main_window);

		// Camera
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		program.uniformMatrix("view", 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(80.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		program.uniformMatrix("projection", 1, GL_FALSE, glm::value_ptr(projection));

		program.use();
		texture.bind();
		
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (int i = 0; i < 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube_positions[i]);
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.8f, 0.2f, 0.5f));
			program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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

	const float camera_speed = 4.0f * delta_time;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_up;
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		camera_pos += camera_speed * camera_up;
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(direction);
}