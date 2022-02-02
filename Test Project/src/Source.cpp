#include "pch.h"
#include <GLFW/glfw3.h>// Include GLFW

#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "Model.h"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const char* TITLE = "Model Loading Test: ";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(char const* path);

const char* vertexPath = "Test Project/resources/shaders/vertex.vert",
* fragmentPath = "Test Project/resources/shaders/frag.frag";

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
		getchar(); exit(EXIT_FAILURE);
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
		getchar(); exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to initialize GLAD");
		getchar(); exit(EXIT_FAILURE);
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
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//stbi_set_flip_vertically_on_load(true);
	Shader shader(vertexPath, fragmentPath);
	//Model ourModel("Test Project/resources/models/suit_model/nanosuit.obj");
	//Model ourModel("Test Project/resources/models/ybot.dae");
	//Model ourModel("Test Project/resources/models/ybot.fbx");
	Model ourModel("Test Project/resources/models/Warrok.obj");
	//Model ourModel("Test Project/resources/models/Warrok.fbx");


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
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.Activate();
		shader.setVec3("camPos", camera.Position);
		shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		shader.setMat4("model", model);
		ourModel.Draw(shader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader.Delete();
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
	//std::cout << lastX << " " << lastY << "\n";

	float xoffset = float(xpos - lastX);
	float yoffset = float(lastY - ypos); // reversed: y ranges bottom to top
	//std::cout << "xoffset: " << xoffset << "\tyoffset: " << yoffset << "\n";
	camera.ProcessMouseMovement(xoffset, yoffset);
	lastX = xpos;
	lastY = ypos;
	//camera.Debug();

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
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
