#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "skybox/skybox.h"
#include "eight-diagram/eight-diagram.h"
#include "temple/temple.h"
#include "book/book.h"
#include "rock/rock.h"
#include "ground/ground.h"
#include "buddha/buddha.h"
#include "bomb/bomb.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(-15.0f, 1.0f, 30.0f));
//Camera camera(glm::vec3(0.0f, 1.0f, 15.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float buddhaHeight = 0.0f;

/*
 * stage:
 *  0: normal state
 *  1: there is an explode request to handle
 *  2: bomb is flying
 */
int stage = 0;
int rockState = 0;  // 0: both alive, 1: rock1 is exploded, 2: rock2 is exploded, 3: both rocks are exploded

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	SkyBox skybox = SkyBox();
	EightDiagram eightDiagram = EightDiagram();
	Temple temple = Temple();
	Book book = Book();
	Ground ground = Ground();
	Buddha buddha = Buddha();
	Rock rock1 = Rock();
	Rock rock2 = Rock();
	Bomb bomb = Bomb();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		if (stage == 1) {
			if (!bomb.inExplodableArea(camera.Position)) {
				stage = 0;
			}
			else {
				bomb.setPos(camera.Position);
				if (camera.Position.z > 0) {
					bomb.setTargetPos(glm::vec3(0.4f, 1.7f, 9.0f));
					if (rockState == 0 || rockState == 2) {
						rockState += 1;
						stage = 2;
					}
					else {
						stage = 0;
					}
				}
				else {
					bomb.setTargetPos(glm::vec3(0.4f, 1.7f, -8.7f));
					if (rockState == 0 || rockState == 1) {
						rockState += 2;
						stage = 2;
					}
					else {
						stage = 0;
					}
				}
			}
		}

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene as normal
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// rock
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, 1.7f, 6.0f));
		model = glm::rotate(model, glm::radians(-75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.33f, 1.33f, 1.33f));
		rock1.drawRock(model, view, projection, camera);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.28f, 1.7f, -6.0f));
		model = glm::rotate(model, glm::radians(-75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));
		rock2.drawRock(model, view, projection, camera);

		// bomb
		if (stage == 2) {
			model = glm::mat4(1.0f);
			//bomb.setPosZ(bomb.getPosZ() - deltaTime * bomb.getVZ());
			bool shouldExplode = bomb.updatePos(deltaTime);
			if (shouldExplode) {
				bomb.startExplode();
				if (bomb.getPos().z > 0)
					rock1.startExplode();
				else
					rock2.startExplode();
				stage = 0;
				buddha.startGlare = true;  // once a rock is exploded, buddha starts to glare
			}
			model = glm::translate(model, bomb.getPos());
			model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			bomb.draw(model, view, projection, camera);
		}

		// temple model
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		temple.drawTemple(model, view, projection, camera);

		// buddha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		if (camera.IsLightOn)
			buddha.accumulateGlare(deltaTime);
		buddha.draw(model, view, projection, camera);

		// eight-diagram
		//eightDiagram.setPos(eightDiagram.getPos() - deltaTime * eightDiagram.getTranslateV());
		//model = glm::translate(model, glm::vec3(eightDiagram.getPos(), 0.0f, -20.0f));
		//model = glm::rotate(model, (float)(glfwGetTime() * eightDiagram.getRotateV()), glm::vec3(0.0f, 0.5f, 1.0f));
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//eightDiagram.drawEightDiagram(model, view, projection);

		// book
		model = glm::mat4(1.0f);
		book.setPosZ(book.getPosZ() - deltaTime * book.getVZ());
		model = glm::translate(model, glm::vec3(-15.5f, 0.5f, book.getPosZ()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		book.drawBook(model, view, projection, camera);
		cout << book.getPosZ() << endl;
		if (book.getPosZ() < -20.0f)
			camera.CanBeMoved = true;

		//camera.ProcessMouseMovement(-deltaTime * 70, 0.0f);  // for more smoothness

		// ground
		ground.draw(glm::mat4(1.0f), view, projection, camera);

		// skybox
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skybox.drawSkybox(view, projection);


	    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	// glDeleteVertexArrays(1, &cubeVAO);
	// glDeleteVertexArrays(1, &skyboxVAO);
	// glDeleteBuffers(1, &cubeVBO);
	// glDeleteBuffers(1, &skyboxVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (camera.CanBeMoved)
			camera.ProcessKeyboard(FORWARD, deltaTime, rockState);
		cout << camera.Position.x << "\t" << camera.Position.y << "\t" << camera.Position.z << "\t" << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (camera.CanBeMoved)
			camera.ProcessKeyboard(BACKWARD, deltaTime, rockState);
		cout << camera.Position.x << "\t" << camera.Position.y << "\t" << camera.Position.z << "\t" << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (camera.CanBeMoved)
			camera.ProcessKeyboard(LEFT, deltaTime, rockState);
		cout << camera.Position.x << "\t" << camera.Position.y << "\t" << camera.Position.z << "\t" << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (camera.CanBeMoved)
			camera.ProcessKeyboard(RIGHT, deltaTime, rockState);
		cout << camera.Position.x << "\t" << camera.Position.y << "\t" << camera.Position.z << "\t" << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		buddhaHeight += 0.005f;
		cout << buddhaHeight << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		buddhaHeight -= 0.005f;
		cout << buddhaHeight << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime, rockState);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime, rockState);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && stage == 0)
		stage = 1;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (glfwGetTime() - camera.LastToggleLight > 0.5f) {
			// cannot toggle light twice in half second
			camera.IsLightOn = !camera.IsLightOn;
			camera.LastToggleLight = glfwGetTime();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//cout << window << '\t' << xpos << endl;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}