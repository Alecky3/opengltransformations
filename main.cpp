#include<glm/glm.hpp>
#include<iostream>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
void resizeCallback(GLFWwindow* window, int width, int height);
void createProgram();
void drawtriangle();
void init();
GLuint VBO[2];
GLuint VAO[2];
GLuint EBO[1];
GLuint programs[1];
int main(int argc, char** argv) {

	if (!glfwInit()) {
		std::cout << "Could not initialize glfw" << std::endl;
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) {
		std::cout << "Could not initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	init();
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.5, 0.3, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		drawtriangle();
		//std::cout << glfwGetTime() << std::endl;
		glm::mat4 trans = glm::mat4(1.0);
		trans = glm::rotate(trans,(float)glfwGetTime(), glm::vec3(0, 0, 1));
		trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0));
		unsigned int transform = glGetUniformLocation(programs[0], "transform");
		glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}

void resizeCallback(GLFWwindow* window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void createProgram()
{
	const char* vertexSource = "#version 330 core\n"
		"in vec2 aPos;\n"
		"uniform mat4 transform;"
		"void main(){\n"
		"gl_Position= transform * vec4(aPos,1.0,1.0);"
		"\n}\0";
	const char* fragmentSource = "#version 330 core\n"
		"out vec4 fragColor;"
		"void main()\n{"
		"fragColor=vec4(1.0,0,0,1.0);"
		"\n}\0";

	//create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error compiling vertex shader : " << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//check for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
	}

	//create program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	programs[0] = program;

	//check for errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Error linking program: " << infoLog << std::endl;
	}
}

void drawtriangle()
{
	glUseProgram(programs[0]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO[0]);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindVertexArray(VAO[1]);
	//glDrawArrays(GL_TRIANGLES, 3, 3);
	/*for (int i = 0; i < 2; i++) {
		glm::mat4 trans = glm::mat4(1.0);
		trans = glm::translate(trans, glm::vec3(0.5*i+1/2, 0.5*i+1/2, 0));
		unsigned int transform = glGetUniformLocation(programs[0], "transform");
		glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	*/
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glBindVertexArray(VAO[1]);

	glm::mat4 trans = glm::mat4(1.0);
	trans = glm::translate(trans, glm::vec3(0.5 ,0.5, 0));
	unsigned int transform = glGetUniformLocation(programs[0], "transform");
	glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));


	//glBindVertexArray(VAO[1]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}

void init()
{
	float vertices[] = {
		-0.5,-0.5,//bottom left
		-0.5,0.5,//top left
		0.5,-0.5,//bottom right
		0.5,0.5//top right
	};
	
	unsigned int indices[]{
		0,1,2,
		1,2,3
	};
	

	/*float vertices[] = {
		-0.5,-0.5,
		-0.5,0.5,
		0.5,-0.5,
		-0.5,0.5,
		0.5,0.5,
		0.5,-0.5,
		0.6,-0.5,
		0.9,0.0,
		0.6,0.5
	};
	*/
	glGenBuffers(2, VBO);

	//ebo
	glGenBuffers(1, &EBO[0]);

	//vao
	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);

	//vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	//ebo
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//get location of attribute
	createProgram();
	glm::mat4 trans = glm::mat4(1.0);
	trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0));
	trans = glm::rotate(trans, glm::radians(60.0f), glm::vec3(0, 0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	

	unsigned int transform = glGetUniformLocation(programs[0], "transform");
	
	glUseProgram(programs[0]);
	glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
	GLint aPos = glGetAttribLocation(programs[0], "aPos");
	//configure vertex data
	glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(aPos);

	//vao
	//glBindVertexArray(VAO[1]);

	//ebo2
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vbo2
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//unsigned int transform1 = glGetUniformLocation(programs[0], "transform");
	//glm::mat4 trans1 = glm::mat4(1.0);
	//trans = glm::translate(trans1, glm::vec3(-0.5, 0.5, 0));
	//glUniformMatrix4fv(transform1, 1, GL_FALSE, glm::value_ptr(trans1));

	//glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	//glEnableVertexAttribArray(aPos);

}
