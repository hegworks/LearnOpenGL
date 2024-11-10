#include <cmath>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

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

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	//----------objects initialization
	//-----points
	constexpr int point_count = 3;
	constexpr int position_parts = 3;
	constexpr int color_parts = 3;
	constexpr size_t stride = (position_parts + color_parts) * sizeof(float);
	constexpr size_t position_offset = 0;
	constexpr size_t color_offset = position_parts * sizeof(float);
	float vertices[6 * 3] =
	{
		// positions			// colors
		0.0f,	0.75f,	0.0f,	1.0f, 0.0f, 0.0f,	// top
		-0.75f,	-0.75f,	0.0f,	0.0f, 1.0f, 0.0f,	// left
		0.75f,	-0.75f,	0.0f,	0.0f, 0.0f, 1.0f	// right
	};
	//=====points

	//-----Initialization
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//=====Initialization

	//-----Binding
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//=====Binding

	// VertexAttribute
	glVertexAttribPointer(0, position_parts, GL_FLOAT, GL_FALSE, stride, (void*)(position_offset));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, color_parts, GL_FLOAT, GL_FALSE, stride, (void*)(color_offset));
	glEnableVertexAttribArray(1);

	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//==========objects initialization

	// Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment out for default behavior


	float deltaTime = 0;
	float horizontalOffset = 0;
	float horizontalOffsetSpeed = 0.5f;
	float horizontalOffsetMax = 1.75f;
	int isFlipped = 1;
	while(!glfwWindowShouldClose(window))
	{
		double frameStartTime = glfwGetTime();

		printf("dt: %f\n", deltaTime);

		// input
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//-----render

		float timeValue = glfwGetTime();
		vertices[3] = sin(timeValue) / 2.0f + 0.5f;
		vertices[10] = cos(timeValue) / 2.0f + 0.5f;
		vertices[17] = -sin(timeValue) / 2.0f + 0.5f;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		horizontalOffset += horizontalOffsetSpeed * deltaTime;
		if(horizontalOffset > horizontalOffsetMax)
		{
			horizontalOffset = -horizontalOffsetMax;
			isFlipped = -isFlipped;
			shader.SetInt("isVerticallyFlipped", isFlipped);
		}
		shader.SetFloat("horizontalOffset", horizontalOffset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, point_count);
		glBindVertexArray(0);

		//=====render

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		double frameEndTime = glfwGetTime();
		deltaTime = static_cast<float>(frameEndTime - frameStartTime);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.Delete();
	glfwTerminate();
	return 0;
}
