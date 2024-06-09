#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void PreSettingGlwf();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {
	PreSettingGlwf();

	GLFWwindow* window{ glfwCreateWindow(800, 600, "LearnOpengl", nullptr, nullptr) };
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, &key_callback);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

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