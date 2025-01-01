#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <memory>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderYellowSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

struct SimpleMesh
{
	float vertices[9];
	unsigned int VAO, VBO;
	unsigned int Shader;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float* processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float res[3] = { 0.2f, 0.3f, 0.3f };

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		res[0] = 0.5f; res[1] = 0.2f; res[2] = 0.15f;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		res[0] = 1.0f; res[1] = 0.0f; res[2] = 0.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		res[0] = 0.0f; res[1] = 1.0f; res[2] = 0.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		res[0] = 0.0f; res[1] = 0.0f; res[2] = 1.0f;
	}

	// check for wireframe
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	return res;
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
	
	/// *** CONFIGURE SHADERS ***
	// compile vertex shader.
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success; char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	// compiler fragment shader.
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	// compiler yellow fragment shader.
	unsigned int fragmentYellowShader;
	fragmentYellowShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentYellowShader, 1, &fragmentShaderYellowSource, NULL);
	glCompileShader(fragmentYellowShader);

	glGetShaderiv(fragmentYellowShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentYellowShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	// link shaders to program.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
			<< infoLog << std::endl;
	}

	unsigned int shaderYellowProgram;
	shaderYellowProgram = glCreateProgram();
	glAttachShader(shaderYellowProgram, vertexShader);
	glAttachShader(shaderYellowProgram, fragmentYellowShader);
	glLinkProgram(shaderYellowProgram);

	glGetProgramiv(shaderYellowProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderYellowProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM_YELLOW::LINK_FAILED\n"
			<< infoLog << std::endl;
	}

	// GL use it!
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentYellowShader);
	unsigned int shaderPrograms[2] = { shaderProgram, shaderYellowProgram };

	/// *** CONFIGURE VERTEX BUFFERS ***
	// gl create vertex buffer.
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// EXERCISE 2: Draw two triangles with different VAO/VBO
	// Use SimpleMesh struct
	float verticesToLoad[2][9] =
	{ 
		{
		  0.125f,  0.125f, 0.0f,
		  0.125f, -0.125f, 0.0f,
		 -0.125f,  0.125f, 0.0f
		},
		
		{
		   0.125f, -0.125f, 0.0f,
		  -0.125f, -0.125f, 0.0f,
		  -0.125f,  0.125f, 0.0f
		}
	};

	std::vector<SimpleMesh> meshes;
	for (GLuint i = 0; i < 2; i++)
	{
		SimpleMesh newMesh;
		newMesh.Shader = shaderYellowProgram; //shaderPrograms[i];

		for (GLuint v = 0; v < 9; v++)
		{
			newMesh.vertices[v] = verticesToLoad[i][v];
		}

		glGenVertexArrays(1, &newMesh.VAO);
		glGenBuffers(1, &newMesh.VBO);

		glBindVertexArray(newMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(newMesh.vertices),
			newMesh.vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,					// Load to vertex attribute 0. 
			3,					// Size of attribute (3 for vec3).
			GL_FLOAT,			// Attribute is float type. 
			GL_FALSE,			// Do not normalize attribute.
			3 * sizeof(float),  // Stride
			(void*)0			// Pointer offset to start of data.
		);
		glEnableVertexAttribArray(0);

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		meshes.push_back(newMesh);
	}

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // GL_ARRAY_BUFFER
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// DRAW TYPES:
	// GL_STREAM_DRAW: set once, use a few times.
	// GL_STATIC_DRAW: set once, used many times.
	// GL_DYNAMIC_DRAW: set many times, used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// link vertex attributes.
	// IMPORTANT:
	//	Data is pulled from whatever data is currently bound to 
	//  GL_ARRAY_BUFFER when this function is called.
	glVertexAttribPointer(
		0,					// Load to vertex attribute 0. 
		3,					// Size of attribute (3 for vec3).
		GL_FLOAT,			// Attribute is float type. 
		GL_FALSE,			// Do not normalize attribute.
		3 * sizeof(float),  // Stride
		(void*)0			// Pointer offset to start of data.
	);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float* color;
	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shaderProgram);

		color = processInput(window);

		// rendering!
		glClearColor(color[0], color[1], color[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 
			6, //sizeof(indices)/sizeof(unsigned int),
			GL_UNSIGNED_INT, 0);

		for (const SimpleMesh & mesh : meshes)
		{
			glUseProgram(mesh.Shader);
			glBindVertexArray(mesh.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// swap buffers, then pull IO events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
