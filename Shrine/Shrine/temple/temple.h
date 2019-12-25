#pragma once
#include <vector>
#include "../shader.h"
#include "../model.h"
#include "../camera.h"

using namespace std;

class Temple {
public:
	Shader templeShader;

	Temple()
		: templeShader(Shader("temple/temple.vs", "temple/temple.fs")),
		  templeModel(Model("resources/objects/temple/Japanese_country_house_3_obj.obj"))
	{}

	void drawTemple(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		templeShader.use();
		templeShader.setMat4("projection", projection);
		templeShader.setMat4("view", view);
		templeShader.setMat4("model", model);
		templeShader.setInt("material.diffuse", 0);
		templeShader.setInt("material.specular", 1);
		templeShader.setFloat("material.shininess", 32.0f);
		templeShader.setVec3("light.position", camera.Position);
		templeShader.setVec3("light.color", 1.0f, 1.0f, 1.0f);
		templeShader.setVec3("light.direction", camera.Front);
		templeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		templeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		templeShader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.1f;
		float diffuseValue = 0.8f;
		float specularValue = 1.0f;
		templeShader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		templeShader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		templeShader.setVec3("light.specular", specularValue, specularValue, specularValue);
		templeShader.setFloat("light.constant", 1.0f);
		templeShader.setFloat("light.linear", 0.09f);
		templeShader.setFloat("light.quadratic", 0.032f);
		templeModel.Draw(templeShader);
	}

private:
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	Model templeModel;
};