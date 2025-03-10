#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	// Initialize GLFW
	glfwInit();

	// Set GLFW hints like the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	// Check if window creating failed
	GLFWwindow* window = glfwCreateWindow(800, 800, "Cpp Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;
	}

	// Make window our context
	glfwMakeContextCurrent(window);

	// Load	GLAD
	gladLoadGL();

	// Set the viewport
	glViewport(0, 0, 800, 800);

	// Set background color
	glClearColor(0.0f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Handle Events
		glfwPollEvents();
	}

	// Clean things up
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}