#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <memory>
#include <Shader.h>

// stbi
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string SHADER_LOC = "C:\\Users\\colin\\source\\repos\\learn_opengl\\LearnOpenGL\\shdr";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		return 0.0001f;
	}

	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		return -0.0001f;
	}
}

int main()
{
	// init glfw and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw create window
	GLFWwindow* window = glfwCreateWindow(800, 600,
		"LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load all OpenGL function pointers with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	/* 
		Setup scene, textures, shaders, etc.
	*/

	// texture behavior config.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		
	// declare min/mag behavior -- we'll nearest sample.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Following load routine would be a suitable constructor for a texture class.
	// *****
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources\\container.jpg", &width, &height, &nrChannels, 0);
	
	unsigned int textureWood;
	glGenTextures(1, &textureWood);
	glBindTexture(GL_TEXTURE_2D, textureWood);

	if (data)
	{
		// After this call, currently bound texture object will have our texture attached.
		glTexImage2D(
			GL_TEXTURE_2D,		//texture target - currently bound to GL_TEXTURE_2D 
			0,					//mipmap level 
			GL_RGB,				//format to store texture -- RGB
			width, height,		//width and height of texture
			0,					// ** ALWAYS ZERO **
			GL_RGB,				//Format of source image
			GL_UNSIGNED_BYTE,   //Stored in char (unsigned bytes)
			data				//Actual image.
		);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	
	// now safe to free image data.
	stbi_image_free(data);

	// load second texture
	//stbi_set_flip_vertically_on_load(true);
	data = stbi_load("resources\\smileyFace.png", &width, &height, &nrChannels, 0);

	unsigned int textureSmile;
	glGenTextures(1, &textureSmile);
	glBindTexture(GL_TEXTURE_2D, textureSmile);

	if (data)
	{
		// After this call, currently bound texture object will have our texture attached.
		glTexImage2D(
			GL_TEXTURE_2D,		//texture target - currently bound to GL_TEXTURE_2D 
			0,					//mipmap level 
			GL_RGB, 			//format to store texture -- RGB
			width, height,		//width and height of texture
			0,					// ** ALWAYS ZERO **
			GL_RGBA,			//Format of source image
			GL_UNSIGNED_BYTE,   //Stored in char (unsigned bytes)
			data				//Actual image.
		);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	// now safe to free image data.
	stbi_image_free(data);


	// End "texture constructor"
	// *****

	// Define a rectangle with colors and texture coordinates.
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	};
	unsigned int indices[]
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// assign vertex and color attributes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// assign texture coord attrib.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Create shader which uses texture.
	Shader textureShader = Shader(SHADER_LOC + "\\v_TEXTURE.glsl", SHADER_LOC + "\\f_TEXTURE.glsl");

	/*
		Render loop!
	*/

	float mixValue = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		mixValue += processInput(window);
		if (mixValue >= 1.0f) { mixValue = 1.0f; }
		else if (mixValue <= 0.0f) { mixValue = 0.0f; }
		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWood);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSmile);

		// render textures
		textureShader.use();
		textureShader.setInt("texture1", 0);
		textureShader.setInt("texture2", 1);
		textureShader.setFloat("mixV", mixValue);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}