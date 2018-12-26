#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char *fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

// 第一个三角形
float vertices_orange[] = {
	-.25f, .5f, 0.0f,
	-.5f, .0f, 0.0f,
	.0f, 0.0f, 0.0f
};
// 第二个三角形
float vertices_yellow[] = {
	.25f, 0.5f, 0.0f,
	0.0f, .0f, 0.0f,
	.5f, .0f, 0.0f
};

void framebuffer_cb(GLFWwindow*, int, int);
void input_process(GLFWwindow*);

void init_vertex_shader(int*, const char * &);
void init_fragment_shader(int*, const char * &);

void init_shader_program(int*, int, int);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "create window fail!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// process func address with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "glad fail!" << endl;
		glfwTerminate();
		return -1;
	}
	// set frame_buffer callback
	glfwSetFramebufferSizeCallback(window, framebuffer_cb);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader;
	init_vertex_shader(&vertexShader, vertexShaderSource);

	// fragment shader
	int fragmentShaderOrange, fragmentShaderYellow;
	init_fragment_shader(&fragmentShaderOrange, fragmentShaderSourceOrange);
	init_fragment_shader(&fragmentShaderYellow, fragmentShaderSourceYellow);

	// link shaders
	int shaderProgramOrange, shaderProgramYellow;
	init_shader_program(&shaderProgramOrange, vertexShader, fragmentShaderOrange);
	init_shader_program(&shaderProgramYellow, vertexShader, fragmentShaderYellow);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);

	GLuint VAO[2];
	glGenVertexArrays(2, VAO);

	GLuint VBO[2];
	glGenBuffers(2, VBO);

	// first triangle data binding
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_orange), vertices_orange, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second triangle data binding
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_yellow), vertices_yellow, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		input_process(window);

		glClearColor(0.0f, 0.0f, 0.5f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//glDeleteBuffers(1, &EBO);

	// terminate
	glfwTerminate();
	return 0;
}

void framebuffer_cb(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void input_process(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void init_vertex_shader(int* vertext_id, const char * &vertexSource)
{
	*vertext_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertext_id, 1, &vertexSource, NULL);
	glCompileShader(*vertext_id);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(*vertext_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*vertext_id, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void init_fragment_shader(int* fragment_id, const char * &fragmentSource)
{
	*fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fragment_id, 1, &fragmentSource, NULL);
	glCompileShader(*fragment_id);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(*fragment_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*fragment_id, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void init_shader_program(int* shader_program_id, int vertex_shader_id, int fragment_shader_id)
{
	*shader_program_id = glCreateProgram();
	glAttachShader(*shader_program_id, vertex_shader_id);
	glAttachShader(*shader_program_id, fragment_shader_id);
	glLinkProgram(*shader_program_id);
}