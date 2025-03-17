#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

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
	GLfloat	vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// Lower left corner
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// Upper left corner
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// Upper right corner
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// Lower right corner
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
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("pop_cat.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear screen
		glClearColor(0.0f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);

		glBindTexture(GL_TEXTURE_2D, texture);	

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
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}