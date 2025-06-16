#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void PreSettingGlwf();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr GLuint WIDTH{ 800 };
constexpr GLuint HEIGHT{ 600 };

// Shaders
const GLchar* vertexShaderSource{ "#version 330 core\n"
								  "layout (location = 0) in vec3 position;\n"
								  "void main()\n"
								  "{\n"
								  "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
								  "}\0" };

const GLchar* fragmentShaderSource{ "#version 330 core\n"
									"out vec4 color;\n"
									"void main()\n"
									"{\n"
									"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\0" };

int main() {
	PreSettingGlwf();

	GLFWwindow* window{ glfwCreateWindow(WIDTH, HEIGHT, "LearnOpengl", nullptr, nullptr) };
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, &key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	// Build and compile our shader program
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[]{
		0.5f,  0.5f,  0.0f,	 //
		0.5f,  -0.5f, 0.0f,	 //
		-0.5f, -0.5f, 0.0f,	 //
		-0.5f, 0.5f,  0.0f,	 //
	};

	GLuint indices[] = {
		0, 1, 3,  //
		1, 2, 3	  //
	};

	// Element Buffer Object (EBO) or Index Buffer Object (IBO)
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// Vertex Array Object (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// Vertex Buffer Object (VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set Vertex Buffer(s) and Attribute pointer(s)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// To copy buffer data
	// - GL_STATIC_DRAW: data that will never or very rarely be changed;
	// - GL_DYNAMIC_DRAW: if data changes frequently;
	// - GL_STREAM_DRAW: if data changed every frame.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// How OpenGL can interpret vertex data (vertices)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound
	//	Vertex Buffer Object so afterward we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response
		// functions
		glfwPollEvents();

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they have outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void PreSettingGlwf() {
	// Initialization GLFW
	glfwInit();

	// Setup GLFW
	// Set minimum required OpenGL version
	// Major
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// Minor
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Setup profile for context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Turn off ability changing window size
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}