#include <cmath>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Common.h"
#include "Shader.h"
#include "STB/stb_image.h"
#include "Tools/GlCheckError.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "Camera/FPSCamera.h"
#include "Camera/FreeFlyCamera.h"

#include "Model/Model.h"
#include <filesystem>

Camera* m_camera = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
	m_camera->ProcessInput(window);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	m_camera->MouseCallback(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		m_camera->KeyDown(key);
	}
	if(action == GLFW_RELEASE)
	{
		m_camera->KeyUp(key);
	}
}

GLFWwindow* WindowSetup()
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.X = 700;
	coninfo.dwSize.Y = 9999;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 0, 700, 1000, 0);
#endif

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_POSITION_X, 700);
	glfwWindowHint(GLFW_POSITION_Y, 100);

	GLFWwindow* window = glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

#if defined(__linux__) && !defined(FULLSCREEN)
	/*
	* based on https://discourse.glfw.org/t/not-able-to-set-window-position/166/9,
	* in order to set glfw window position correctly on linux, you have to do it twice!
	*/
	glfwSetWindowPos(window, 0, 50);
	glfwShowWindow(window);
	glfwSetWindowSize(window, SCRWIDTH, SCRHEIGHT);
	glfwSetWindowPos(window, 0, 50);
#endif // defined(__linux__) && !defined(FULLSCREEN)

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#ifdef FULLSCREEN
	glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 144);
#endif // FULLSCREEN

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	return window;
}

int main(int argc, char* argv[])
{
	printf("Hello world\n");

	GLFWwindow* window = WindowSetup();
	if(window == nullptr) return -1;

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	m_camera = new FPSCamera();

	stbi_set_flip_vertically_on_load(true);

	//----------objects initialization
	//-----points
	constexpr int point_count = 36;
	constexpr int position_parts = 3;
	constexpr int color_parts = 0;
	constexpr int texture_parts = 2;

	constexpr size_t stride = (position_parts + color_parts + texture_parts) * sizeof(float);
	constexpr size_t position_offset = (0) * sizeof(float);
	constexpr size_t color_offset = (position_parts) * sizeof(float);
	constexpr size_t texture_offset = color_offset + color_parts * sizeof(float);
	float vertices[] = {
		//position			  //textureCoords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] =
	{
	glm::vec3(0.0f,			0.0f,	0.0f),
	glm::vec3(1.0f * 2.0f,	0.0f,	-1.0f * 2.0f),
	glm::vec3(2.0f * 2.0f,	0.0f,	-1.0f * 2.0f),
	glm::vec3(3.0f * 2.0f,	0.0f,	-1.0f * 2.0f),
	glm::vec3(1.0f * 2.0f,	0.0f,	-2.0f * 2.0f),
	glm::vec3(2.0f * 2.0f,	0.0f,	-2.0f * 2.0f),
	glm::vec3(3.0f * 2.0f,	0.0f,	-2.0f * 2.0f),
	glm::vec3(1.0f * 2.0f,	0.0f,	-3.0f * 2.0f),
	glm::vec3(2.0f * 2.0f,	0.0f,	-3.0f * 2.0f),
	glm::vec3(3.0f * 2.0f,	0.0f,	-3.0f * 2.0f)
	};
	//=====points

	//-----Initialization
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//=====Initialization

	//-----Binding
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glCheckError();
	//=====Binding

	// VertexAttribute
	glVertexAttribPointer(0, position_parts, GL_FLOAT, GL_FALSE, stride, (void*)(position_offset));
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, color_parts, GL_FLOAT, GL_FALSE, stride, (void*)(color_offset));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, texture_parts, GL_FLOAT, GL_FALSE, stride, (void*)(texture_offset));
	glEnableVertexAttribArray(2);

	// UnBinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//==========objects initialization

	//----------other options
	// Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment out for default behavior

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//==========other options

	float deltaTime = 0;
	float mixValue = 0.5f;
	const float mixChangeSpeed = 0.6f;
	constexpr glm::mat4 identity = glm::mat4(1.0f);

	Shader modelShader = Shader("src/Shaders/Model.vert", "src/Shaders/Model.frag");
	glCheckError();
	modelShader.Use();
	Model* backpack = new Model("Assets/Models/Backpack/backpack.obj");


	while(!glfwWindowShouldClose(window))
	{
		const double frameStartTime = glfwGetTime();

		// input
		processInput(window);
		//if(isDownKeyPressed || isUpKeyPressed)
		//{
		//	mixValue += isDownKeyPressed ? -mixChangeSpeed * deltaTime : +mixChangeSpeed * deltaTime;
		//	mixValue = mixValue > 1.0f ? 1.0f : mixValue;
		//	mixValue = mixValue < 0.0f ? 0.0f : mixValue;
		//	shader.SetFloat("uMix", mixValue);
		//}

		//-----Camera
		m_camera->Update(deltaTime);
		glm::vec3 cameraPos, cameraFront, cameraUp;
		m_camera->GetCameraProperties(cameraPos, cameraFront, cameraUp);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//=====Camera

		glm::mat4 projection = identity;
		projection = glm::perspective(glm::radians(m_camera->GetFov()), 16.0f / 9.0f, 0.1f, 100.0f);

		//-----render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//===Backpack
		modelShader.Use();

		glUniformMatrix4fv(modelShader.GetUniformLocation("uView"), 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(modelShader.GetUniformLocation("uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 bpModel = identity;
		bpModel = glm::translate(bpModel, glm::vec3(-2, 2, -2));
		bpModel = glm::scale(bpModel, glm::vec3(0.5));
		glUniformMatrix4fv(modelShader.GetUniformLocation("uModel"), 1, GL_FALSE, glm::value_ptr(bpModel));
		backpack->Draw(modelShader);
		//---Backpack

		//=====transformations

		//=====render

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		const double frameEndTime = glfwGetTime();
		deltaTime = static_cast<float>(frameEndTime - frameStartTime);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	modelShader.Delete();
	glfwTerminate();
	return 0;
}
