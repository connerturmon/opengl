#include <iostream>
#include <cmath>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Cube.h"
#include "Texture.h"
#include "Camera.h"
#include "CubeInst.h"

const unsigned int WIN_WIDTH = 1920;
const unsigned int WIN_HEIGHT = 1080;

void glfwErrorCallback(int code, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = WIN_WIDTH / 2, last_y = WIN_HEIGHT / 2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool release_mouse = false;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	glfwSetFramebufferSizeCallback(main_window, framebufferSizeCallback);
	glfwSetKeyCallback(main_window, glfwKeyCallback);
	glfwSetCursorPosCallback(main_window, mouseCallback);

	// IMGUI SETUP
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(main_window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

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

	GLfloat plane_verts[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	GLuint plane_vbo;
	glGenBuffers(1, &plane_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_verts), plane_verts, GL_STATIC_DRAW);
	GLuint plane_vao;
	glGenVertexArrays(1, &plane_vao);
	glBindVertexArray(plane_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
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

	// FUN VARIABLES
	bool translated = false;
	float light_pos_raw[3] = {1.0f, 1.0f, 1.0f};
	float light_color_raw[3] = {1.0f, 1.0f, 1.0f};
	float clear_color[3] = {0.1f, 0.1f, 0.1f};
	bool show_wireframe = false;
	float camera_fov = 90.0f;
	float cube_scale = 1.0f;
	float new_cube_pos[3] = {0.0f, 0.0f, 0.0f};
	float new_cube_color[3] = {0.5f, 0.5f, 0.5f};
	bool generate_cube = false;
	bool generated = false;

	std::vector<CubeInst> cube_list;

	// MAIN LOOP
	while (!glfwWindowShouldClose(main_window))
	{
		glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();
		// ImGui Window
		{
			ImGui::Begin("Tools");

			ImGui::Text("Light Properties:");
			ImGui::DragFloat3("Position##light", light_pos_raw, 0.05f, -20.0f, 20.0f, "%.2f");
			ImGui::ColorEdit3("Color##light", light_color_raw);

			ImGui::Text("Camera Properties:");
			ImGui::SliderFloat("FOV", &camera_fov, 50.0f, 120.0f, "%.1f");

			ImGui::Text("Cube Creation:");
			ImGui::SliderFloat3("Position##cube", new_cube_pos, -2.0f, 2.0f);
			ImGui::ColorEdit3("Color##cube", new_cube_color);
			generate_cube = ImGui::Button("Create");

			ImGui::Text("Cube Properties:");
			ImGui::SliderFloat("Scale", &cube_scale, 0.1f, 3.0f, "%.1f");

			ImGui::Text("Other Properties:");
			ImGui::ColorEdit3("Background", clear_color);
			ImGui::Checkbox("Show Wireframe", &show_wireframe);

			ImGui::End();
		}

		show_wireframe ?
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		processInput(main_window);
		if (!release_mouse)
			glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// float light_z = sin(glfwGetTime() * 1.5) * 3.0;
		// float light_x = cos(glfwGetTime() * 1.5) * 3.0;
		glm::vec3 light_pos = glm::vec3(light_pos_raw[0], light_pos_raw[1], light_pos_raw[2]);
		glm::vec3 cube_color = glm::vec3(0.8f, 0.1f, 0.0f);
		glm::vec3 light_color = glm::vec3(light_color_raw[0], light_color_raw[1], light_color_raw[2]);

		lighting_program.use();

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.viewMatrix();
		lighting_program.uniformMatrix("view", 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera_fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		lighting_program.uniformMatrix("projection", 1, GL_FALSE, glm::value_ptr(projection));
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(cube_scale, cube_scale, cube_scale));
		lighting_program.uniform3f("cube_color", cube_color);
		lighting_program.uniform3f("light_color", light_color);
		lighting_program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
		lighting_program.uniform3f("light_pos", light_pos);
		glm::vec3 camera_position = camera.getPosition();
		lighting_program.uniform3f("view_pos", camera_position);
		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		CubeInst blue_cube(
			glm::vec3(-1.3f, -0.5f, 1.7f),
			glm::vec3(0.1f, 0.2f, 0.9f),
			cube_scale, lighting_program, cube_vao);
		blue_cube.Render();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 0.0f));
		lighting_program.uniform3f("cube_color", glm::vec3(1.0f, 1.0f, 1.0f));
		lighting_program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(plane_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(cube_vao);

		if (generate_cube)
		{
			CubeInst new_cube(
				glm::vec3(new_cube_pos[0], new_cube_pos[1], new_cube_pos[2]),
				glm::vec3(new_cube_color[0], new_cube_color[1], new_cube_color[2]),
				1.0f,
				lighting_program,
				cube_vao);
			cube_list.push_back(new_cube);
		}
		for (std::vector<CubeInst>::iterator it = cube_list.begin(); it != cube_list.end(); it++)
			it->Render();

		light_program.use();
		light_program.uniformMatrix("projection", 1, GL_FALSE, glm::value_ptr(projection));
		light_program.uniformMatrix("view", 1, GL_FALSE, glm::value_ptr(view));
		light_program.uniform3f("light_color", light_color);
		model = glm::mat4(1.0f);
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		light_program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(light_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(main_window);
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

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
	if (!release_mouse)
	{
		float xoffset = xpos - last_x;
		float yoffset = last_y - ypos;
		last_x = xpos;
		last_y = ypos;

		camera.processMouseInput(xoffset, yoffset);
	}
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		release_mouse = release_mouse ? false : true;
}