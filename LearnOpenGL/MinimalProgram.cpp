#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <map>
#include <memory>
#include <Shader.h>
#include <Camera.h>
#include <WindowState.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stbi
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string SHADER_LOC = "C:\\Users\\colin\\source\\repos\\learn_opengl\\LearnOpenGL\\shdr";

// Global state retrieved from here.
WindowState state;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Declare control
const std::map<GLint, Camera_Movement> controlAtlas =
{
	{GLFW_KEY_W,			Camera_Movement::FORWARD	},
	{GLFW_KEY_A,			Camera_Movement::LEFT		},
	{GLFW_KEY_S,			Camera_Movement::BACKWARD	},
	{GLFW_KEY_D,			Camera_Movement::RIGHT		},
	{GLFW_KEY_SPACE,		Camera_Movement::UP			},
	{GLFW_KEY_LEFT_SHIFT,   Camera_Movement::DOWN		},
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	state.WIDTH = width;
	state.HEIGHT = height;
}

void process_mouse(GLFWwindow* window, Camera* camera, WindowState & state,
	double xpos, double ypos)
{
	if (state.FIRST_MOUSE)
	{
		state.LAST_X = xpos;
		state.LAST_Y = ypos;
		state.FIRST_MOUSE = false;
	}

	camera->ProcessMouseMovement(xpos - state.LAST_X, state.LAST_Y - ypos);
	state.LAST_X = xpos;
	state.LAST_Y = ypos;
}

void process_scroll(GLFWwindow* window, Camera* camera, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	process_mouse(window, &camera, state, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	process_scroll(window, &camera, xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Declaring the basic WASD movement
	for (const auto& binding : controlAtlas)
	{
		if (glfwGetKey(window, binding.first) == GLFW_PRESS)
			camera.ProcessKeyboard(binding.second, state.DELTA_TIME);
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
	GLFWwindow* window = glfwCreateWindow(state.WIDTH, state.HEIGHT,
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

	/* Declare interaction behavior. */
	glViewport(0, 0, state.WIDTH, state.HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);

	/* Declare simple model to render (for basic interaction) */
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// assign vertex and color attributes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// assign texture coord attrib.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* Set up basic shader with camera manipulation. */
	Shader textureShader = Shader(SHADER_LOC + "\\v_COORDINATE.glsl", 
								  SHADER_LOC + "\\f_TEXTURE.glsl");

	/* Set up texture -- hide for now, since we should move to a new class later. */
	#pragma region Texture Init
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureWood);
	#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update projection for current window and FOV
		glm::mat4 projection
			= glm::perspective(glm::radians(camera.Zoom),
				(float)width / (float)height, 0.1f, 100.0f);

		textureShader.use();
		textureShader.setInt("texture1", 0);
		textureShader.setMat4("model", glm::mat4(1.0f));
		textureShader.setMat4("view", camera.GetViewMatrix());
		textureShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// End of rendering, swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();

		// update framerate info
		float currentFrame = glfwGetTime();
		state.DELTA_TIME = currentFrame - state.LAST_FRAME;
		state.LAST_FRAME = currentFrame;
	}

	glfwTerminate();
	return 0;
}