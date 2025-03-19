#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "texture.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"


int main()
{
	// Initialize GLFW
	glfwInit();

	// Set GLFW hints like the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Triangle vertices
	GLfloat vertices[] = 
	{
		100.0f, 100.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
		100.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // Top-left
		300.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // Top-right
		300.0f, 100.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f   // Bottom-right
	};


	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2,
	};

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

	// Create Shaderprogram
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();

	// Correct View
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	// Create and bind VAO and VBO and EBO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture
	
	Texture popcat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popcat.texUnit(shaderProgram, "tex", 0);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear screen
		glClearColor(0.0f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);

		popcat.Bind();

		// Bind VAO
		VAO1.Bind();

		// Actually draw the triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers
		glfwSwapBuffers(window);


		// Handle Events
		glfwPollEvents();
	}

	// Clean things up
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popcat.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}