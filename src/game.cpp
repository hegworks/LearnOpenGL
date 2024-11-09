#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0"
;

const char* orangeFragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"
;

const char* greenFragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0"
;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

unsigned int InitializeGreenShaderProgram()
{
	//-----VertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// compilation check
	int vertexShaderSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
	if(!vertexShaderSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====VertexShader

	//-----GreenFragmentShader
	unsigned int greenFragmentShader;
	greenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(greenFragmentShader, 1, &greenFragmentShaderSource, NULL);
	glCompileShader(greenFragmentShader);

	//// compilation check
	int greenFragmentShaderSuccess;
	glGetShaderiv(greenFragmentShader, GL_COMPILE_STATUS, &greenFragmentShaderSuccess);
	if(!greenFragmentShaderSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(greenFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::GREEN::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====GreenFragmentShader

	//-----GreenShaderProgram
	unsigned int greenShaderProgram;
	greenShaderProgram = glCreateProgram();
	glAttachShader(greenShaderProgram, vertexShader);
	glAttachShader(greenShaderProgram, greenFragmentShader);
	glLinkProgram(greenShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(greenFragmentShader);

	// compilation check
	int greenShaderProgramSuccess;
	glGetProgramiv(greenShaderProgram, GL_LINK_STATUS, &greenShaderProgramSuccess);
	if(!greenShaderProgramSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(greenShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====GreenShaderProgram

	return greenShaderProgram;
}

unsigned int InitializeOrangeShaderProgram()
{
	//-----VertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// compilation check
	int vertexShaderSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
	if(!vertexShaderSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====VertexShader

	//-----OrangeFragmentShader
	unsigned int orangeFragmentShader;
	orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
	glCompileShader(orangeFragmentShader);

	// compilation check
	int orangeFragmentShaderSuccess;
	glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &orangeFragmentShaderSuccess);
	if(!orangeFragmentShaderSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====OrangeFragmentShader

	//-----OrangeShaderProgram
	unsigned int orangeShaderProgram;
	orangeShaderProgram = glCreateProgram();
	glAttachShader(orangeShaderProgram, vertexShader);
	glAttachShader(orangeShaderProgram, orangeFragmentShader);
	glLinkProgram(orangeShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(orangeFragmentShader);

	//// compilation check
	int orangeShaderProgramSuccess;
	glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &orangeShaderProgramSuccess);
	if(!orangeShaderProgramSuccess)
	{
		char infoLog[512];
		glGetShaderInfoLog(orangeShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//=====OrangeShaderProgram

	return orangeShaderProgram;
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

	unsigned int orangeShaderProgram = InitializeOrangeShaderProgram();
	unsigned int greenShaderProgram = InitializeGreenShaderProgram();

	//----------objects initialization
	//-----points
	constexpr int point_count = 3;
	constexpr int floats_per_vertex = 3;
	float vertices0[3 * 3] =
	{
	 -0.5f, 0.5f, 0.0f,
	-0.75f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	};
	float vertices1[point_count * 3] =
	{
	0.5f, 0.5f, 0.0f,
	0.75f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	};
	//=====points

	//-----Initialization
	constexpr int triangle_count = 2;

	unsigned int VAOs[triangle_count];
	glGenVertexArrays(triangle_count, VAOs);

	unsigned int VBOs[triangle_count];
	glGenBuffers(triangle_count, VBOs);
	//=====Initialization

	//-----Binding
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	glVertexAttribPointer(0, floats_per_vertex, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, floats_per_vertex, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(0);
	//=====Binding

	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//==========objects initialization

	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment out for default behavior

	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//-----render

		glUseProgram(orangeShaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, point_count);

		glUseProgram(greenShaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, point_count);

		glBindVertexArray(0);

		//=====render

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(orangeShaderProgram);
	glDeleteProgram(greenShaderProgram);
	glfwTerminate();
	return 0;
}
