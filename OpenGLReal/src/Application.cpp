
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define ASSERT(x) if (x) __debugbreak();
#define GLLog(x) ClearError();\
                 x;\
                 ASSERT(CheckError())


static void ClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool CheckError()
{
	if (glGetError() == GL_NO_ERROR)
	{
		return false;
	}
	return true;
}

static std::string ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::string shader = "";

	while (getline(stream, line))
	{
		shader += line;
		shader += "\n";
	}
	return shader;
}

static unsigned int CompileShader(const std::string& shader, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = shader.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	char infoLog[512];

	if (result == GL_FALSE)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader compilation failed" << infoLog << std::endl;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[12] = {
	-0.5f, -0.5f,
	+0.5f, -0.5f,
	+0.5f, +0.5f,
	-0.5f, +0.5f
	};


	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};
	unsigned int VBO, IBO, VAO;
	//Gen vertex array
	GLLog(glGenVertexArrays(1, &VAO));

	// Binding vertex array will 'save' upcoming 
	// bind calls and vertex attrib pointer calls 
	GLLog(glBindVertexArray(VAO));


	VertexBuffer vb(positions, sizeof(positions));
	IndexBuffer ib(indices, 6);


	//position attrib
	GLLog(glEnableVertexAttribArray(0));
	GLLog(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	std::string vertexShader = ParseShader("res/shaders/vertex.shader");
	std::string fragmentShader = ParseShader("res/shaders/fragment.shader");


	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Max # of vertex attributes is: " << nrAttributes << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		float blueVal = sin((time) / 2.0f) + 0.2f;
		int location = glGetUniformLocation(shader, "u_Color");
		glUniform4f(location, 0.5f, 0.0f, blueVal, 1.0f);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		GLLog(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}

