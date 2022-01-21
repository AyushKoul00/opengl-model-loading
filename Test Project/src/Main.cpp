#include "pch.h"
#include <GLFW/glfw3.h>// Include GLFW

#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const char* TITLE = "Multiple Lights: ";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(char const* path);

const char* vertexPath = "resources/shaders/vertex3.vert",
* fragmentPath = "resources/shaders/frag5.frag",
* lightVert = "resources/shaders/lightVert.vert",
* lightFrag = "resources/shaders/lightFrag.frag";

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	#pragma region Initialization
	if (!glfwInit())
	{
		spdlog::error("Failed to initialize GLFW");
		getchar();
		getchar();exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) 
	{
		spdlog::error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.");
		getchar();
		glfwTerminate();
		getchar();exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to initialize GLAD");
		getchar();exit(EXIT_FAILURE);
	}

	std::string gl_version = std::string((const char*)glGetString(GL_VERSION));

	spdlog::info("Initialization Complete. Using OpenGL Version: " + gl_version);
	#pragma endregion
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSwapInterval(0);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	#pragma region VertexData
	float vertices[] = {
		//Position				//Normals			//UVs
	//Front Face
	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		//Tri 1
	1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		//Tri 2
	1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,

	//Back Face
	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,		//Tri 3
	0.0f, 0.0f, 0.0f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,		//Tri 4
	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,

	//Left Face
	0.0f, 0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		//Tri 5
	0.0f, 0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		//Tri 6
	0.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,

	//Right Face
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		//Tri 7
	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		//Tri 8
	1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,

	//Top Face
	0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		//Tri 9
	1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		//Tri 10
	1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,

	//Bottom Face
	0.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,		//Tri 11
	1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,		//Tri 12
	1.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	VAO vao;
	vao.Bind();

	VBO vbo(vertices, sizeof(vertices));

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	vao.Unbind();
	vbo.Unbind();
	#pragma endregion


	#pragma region Texture
	stbi_set_flip_vertically_on_load(true);
	unsigned int diffuseMap = loadTexture("resources/textures/diffuse.png");
	unsigned int specularMap = loadTexture("resources/textures/specular.png");
	#pragma endregion

	float dist = 3.0f;
	glm::vec3 lightPos(-2.0f, 0.0f, 0.0f);
	glm::vec3 cubePos(dist, 0.0f, 0.0f);

	glm::mat4 model, view, projection;
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	Shader shader(vertexPath, fragmentPath), lightShader(lightVert, lightFrag);
	shader.Activate();
	shader.setMat4("projection", projection);

	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setFloat("material.shininess", 32.0f);
	shader.setVec2("Resolution", (float)SCR_WIDTH, (float)SCR_HEIGHT);

	lightShader.Activate();
	lightShader.setMat4("projection", projection);


	float refreshFPS = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		//float currentFrame = static_cast<float>(glfwGetTime());
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (currentFrame - refreshFPS > 1.0f / 2.0f)
		{
			auto title = TITLE + std::to_string(1.0f / deltaTime);
			glfwSetWindowTitle(window, title.c_str());
			refreshFPS = currentFrame;
		}

		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		view = camera.GetViewMatrix();

		shader.Activate();

		shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		shader.setVec3("pointLights[0].position", pointLightPositions[0]);
		shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09f);
		shader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		shader.setVec3("pointLights[1].position", pointLightPositions[1]);
		shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[1].constant", 1.0f);
		shader.setFloat("pointLights[1].linear", 0.09f);
		shader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		shader.setVec3("pointLights[2].position", pointLightPositions[2]);
		shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[2].constant", 1.0f);
		shader.setFloat("pointLights[2].linear", 0.09f);
		shader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		shader.setVec3("pointLights[3].position", pointLightPositions[3]);
		shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[3].constant", 1.0f);
		shader.setFloat("pointLights[3].linear", 0.09f);
		shader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		shader.setVec3("spotLight.position", camera.Position);
		shader.setVec3("spotLight.direction", camera.Front);
		shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		shader.setVec3("camPos", camera.Position);
		shader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePos);
		shader.setMat4("model", model);

		vao.Bind();
		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);


		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(*vertices));
		}

		//light object
		lightShader.Activate();
		lightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, lightPos);
		lightShader.setMat4("model", model);

		vao.Bind();
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			lightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(*vertices));
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vao.Delete();
	vbo.Delete();
	shader.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = 10.0f;
	else
		camera.MovementSpeed = 2.5f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static double lastX = SCR_WIDTH / 2.0, lastY = SCR_HEIGHT / 2.0;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed: y ranges bottom to top
	camera.ProcessMouseMovement(xoffset, yoffset);
	lastX = xpos;
	lastY = ypos;
}

// utility function for loading a 2D texture from file
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		spdlog::error("Texture failed to load at path: " + std::string(path));
		stbi_image_free(data);
		getchar();exit(EXIT_FAILURE);
	}

	return textureID;
}
