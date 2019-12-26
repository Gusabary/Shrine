#pragma once
#include <vector>
#include "../shader.h"
#include "../model.h"
#include "../camera.h"

using namespace std;

class Rock {
public:
	Shader rockShader;

	Rock()
		: rockShader(Shader("rock/rock.vs", "rock/rock.fs", "rock/rock.gs")),
		rockModel(Model("resources/objects/rock/rockkk.obj")),
		startTime(glfwGetTime())
	{}

	void drawRock(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		rockShader.use();
		rockShader.setMat4("projection", projection);
		rockShader.setMat4("view", view);
		rockShader.setMat4("model", model);
		rockShader.setInt("material.diffuse", 0);
		rockShader.setInt("material.specular", 1);
		rockShader.setFloat("material.shininess", 32.0f);
		rockShader.setVec3("light.position", camera.Position);
		rockShader.setVec3("light.color", 1.0f, 1.0f, 1.0f);
		rockShader.setVec3("light.direction", camera.Front);
		rockShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		rockShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		rockShader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.1f;
		float diffuseValue = 0.8f;
		float specularValue = 1.0f;
		rockShader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		rockShader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		rockShader.setVec3("light.specular", specularValue, specularValue, specularValue);
		rockShader.setFloat("light.constant", 1.0f);
		rockShader.setFloat("light.linear", 0.09f);
		rockShader.setFloat("light.quadratic", 0.032f);
		rockShader.setFloat("time", glfwGetTime() - startTime);
		rockModel.Draw(rockShader);
	}

private:
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	Model rockModel;
	const float startTime;
};