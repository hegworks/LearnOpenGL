#include <cmath>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

void MakeContainer(Shader& shader, unsigned int* vao, unsigned int* vbo, unsigned int* texture0, unsigned int* texture1)
{
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
	//=====points

	unsigned int containerTexture;
	glGenTextures(1, &containerTexture);
	*texture0 = containerTexture;

	unsigned int awesomefaceTexture;
	glGenTextures(1, &awesomefaceTexture);
	*texture1 = awesomefaceTexture;
	//=====

	//-----
	glBindTexture(GL_TEXTURE_2D, containerTexture);
	float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* containerTextureData = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);
	if(!containerTextureData)
	{
		std::cout << "ERROR::STBI::LOAD Failed to load texture: Assets/container.jpg" << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerTextureData);
	glCheckError();
	glGenerateMipmap(GL_TEXTURE_2D);
	//=====

	//-----
	glBindTexture(GL_TEXTURE_2D, awesomefaceTexture);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* awesomefaceTextureData = stbi_load("Assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if(!awesomefaceTextureData)
	{
		std::cout << "ERROR::STBI::LOAD Failed to load texture: Assets/awesomeface.png" << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, awesomefaceTextureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//=====

	//-----
	shader.Use();
	shader.SetInt("uTexture0", 0);
	shader.SetInt("uTexture1", 1);
	//=====

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(containerTextureData);

	//-----Initialization
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	*vao = VAO;

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	*vbo = VBO;
	//=====Initialization

	//-----Binding
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//=====Binding

	// VertexAttribute
	glVertexAttribPointer(0, position_parts, GL_FLOAT, GL_FALSE, stride, (void*)(position_offset));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, texture_parts, GL_FLOAT, GL_FALSE, stride, (void*)(texture_offset));
	glEnableVertexAttribArray(2);

	// UnBinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//==========objects initialization
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

	unsigned int VAO, VBO, texture0, texture1;
	Shader containerShader = Shader("src/Shaders/Vertex.vert", "src/Shaders/Fragment.frag");
	MakeContainer(containerShader, &VAO, &VBO, &texture0, &texture1);

	//----------other options
	// Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment out for default behavior

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//==========other options

	Shader backpackShader = Shader("src/Shaders/Model.vert", "src/Shaders/Model.frag");
	Model* backpack = new Model("Assets/Models/Backpack/backpack.obj");

	float deltaTime = 0;
	constexpr glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 model = identity;
	glm::mat4 view = identity;
	glm::mat4 projection = identity;
	while(!glfwWindowShouldClose(window))
	{
		const double frameStartTime = glfwGetTime();

		processInput(window);

		//--View
		view = identity;
		m_camera->Update(deltaTime);
		glm::vec3 cameraPos, cameraFront, cameraUp;
		m_camera->GetCameraProperties(cameraPos, cameraFront, cameraUp);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//==View

		//==Projection
		projection = identity;
		projection = glm::perspective(glm::radians(m_camera->GetFov()), 16.0f / 9.0f, 0.1f, 100.0f);
		//--Projection

		//----render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//==Container
		containerShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);

		containerShader.SetMat4("uView", view);
		containerShader.SetMat4("uProjection", projection);


		for(int i = -25; i < 25; i++)
		{
			for(int j = -25; j < 25; j++)
			{
				model = identity;
				model = glm::translate(model, glm::vec3(j, 0, i));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				containerShader.SetMat4("uModel", model);
			}
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//==Container

		//==Backpack
		backpackShader.Use();
		backpackShader.SetMat4("uView", view);
		backpackShader.SetMat4("uProjection", projection);

		model = identity;
		model = glm::translate(model, glm::vec3(-2, 2, -2));
		model = glm::scale(model, glm::vec3(0.5));
		backpackShader.SetMat4("uModel", model);

		backpack->Draw(backpackShader);
		//--Backpack

		//====render

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		const double frameEndTime = glfwGetTime();
		deltaTime = static_cast<float>(frameEndTime - frameStartTime);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	backpackShader.Delete();
	containerShader.Delete();
	glfwTerminate();
	return 0;
}
