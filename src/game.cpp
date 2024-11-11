#include <cmath>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "STB/stb_image.h"

#ifdef _WIN32
#include <windows.h>
#endif

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch(errorCode)
		{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case 1283:							   error = "STACK_OVERFLOW"; break;
			case 1284:							   error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << "GL ERROR: " << errorCode << " | " << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool isUpKeyPressed = false;
bool isDownKeyPressed = false;
void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		isUpKeyPressed = true;
	}
	else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
	{
		isUpKeyPressed = false;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		isDownKeyPressed = true;
	}
	else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		isDownKeyPressed = false;
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));

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

	Shader shader = Shader("src/Shaders/Vertex.vert", "src/Shaders/Fragment.frag");
	shader.Use();

	//----------texture

	//-----
	unsigned int containerTexture;
	glGenTextures(1, &containerTexture);

	unsigned int awesomefaceTexture;
	glGenTextures(1, &awesomefaceTexture);
	//=====

	//-----
	glBindTexture(GL_TEXTURE_2D, containerTexture);
	float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	unsigned char* containerTextureData = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);
	if(!containerTextureData)
	{
		std::cout << "Failed to load container.jpg texture" << std::endl;
		return -1;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* awesomefaceTextureData = stbi_load("Assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if(!awesomefaceTextureData)
	{
		std::cout << "Failed to load awesomeface.png texture" << std::endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, awesomefaceTextureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//=====

	//-----
	shader.SetInt("uTexture0", 0);
	shader.SetInt("uTexture1", 1);
	//=====

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(containerTextureData);

	//==========texture

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
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//unsigned int indices[] =
	//{
	//	1,2,3,
	//	0,1,3,
	//};
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
	while(!glfwWindowShouldClose(window))
	{
		const double frameStartTime = glfwGetTime();

		// input
		processInput(window);
		if(isDownKeyPressed || isUpKeyPressed)
		{
			mixValue += isDownKeyPressed ? -mixChangeSpeed * deltaTime : +mixChangeSpeed * deltaTime;
			mixValue = mixValue > 1.0f ? 1.0f : mixValue;
			mixValue = mixValue < 0.0f ? 0.0f : mixValue;
			shader.SetFloat("uMix", mixValue);
		}

		//-----render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, awesomefaceTexture);

		glBindVertexArray(VAO);

		//-----transformations
		for(int i = 0; i < 10; ++i)
		{
			glm::mat4 model = identity;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (i + 1)), glm::vec3(0.1 * i, ((i % 2) == 0 ? 1 : -1) * 0.1 * i, 0.0f));
			glUniformMatrix4fv(shader.GetUniformLocation("uModel"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 view = identity;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		glUniformMatrix4fv(shader.GetUniformLocation("uView"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = identity;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(shader.GetUniformLocation("uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
		//=====transformations


		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

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
	shader.Delete();
	glfwTerminate();
	return 0;
}
