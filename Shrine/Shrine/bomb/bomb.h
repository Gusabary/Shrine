#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "../shader.h"
#include "../model.h"
#include "../camera.h"

using namespace std;

class Bomb {
public:
	Shader shader;

	Bomb()
		: shader(Shader("bomb/bomb.vs", "bomb/bomb.fs", "bomb/bomb.gs")),
		bombModel(Model("resources/objects/bomb/Bomb.obj")),
		startTime(glfwGetTime()), doExplode(false)
	{}

	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);
		shader.setFloat("material.shininess", 32.0f);
		shader.setVec3("light.position", camera.Position);
		shader.setVec3("light.color", 1.0f, 1.0f, 1.0f);
		shader.setVec3("light.direction", camera.Front);
		shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		shader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.1f;
		float diffuseValue = 0.8f;
		float specularValue = 1.0f;
		shader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		shader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		shader.setVec3("light.specular", specularValue, specularValue, specularValue);
		shader.setFloat("light.constant", 1.0f);
		shader.setFloat("light.linear", 0.09f);
		shader.setFloat("light.quadratic", 0.032f);
		shader.setFloat("time", glfwGetTime() - startTime);
		shader.setBool("doExplode", this->doExplode);
		bombModel.Draw(shader);
	}

	void startExplode() {
		this->doExplode = true;
		this->startTime = glfwGetTime();
	}

private:
	unsigned int VAO, VBO;
	unsigned int cubemapTexture;
	Model bombModel;
	float startTime;
	bool doExplode;
};