#pragma once
#include <vector>
#include "../shader.h"
#include "../model.h"
#include "../camera.h"

using namespace std;

class Buddha {
public:
	Shader shader;

	Buddha()
		: shader(Shader("buddha/buddha.vs", "buddha/buddha.fs")),
		  buddhaModel(Model("resources/objects/buddha/12334_statue_v1_l3.obj"))
	{}

	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		shader.use();
		shader.setVec3("cameraPos", camera.Position);
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
		shader.setFloat("light.outerCutOff", glm::cos(glm::radians(30.0f)));
		shader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.3f;
		float diffuseValue = camera.IsLightOn ? 0.8f : 0.0f;
		float specularValue = camera.IsLightOn ? 1.0f : 0.0f;
		shader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		shader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		shader.setVec3("light.specular", specularValue, specularValue, specularValue);
		shader.setFloat("light.constant", 1.0f);
		shader.setFloat("light.linear", 0.09f);
		shader.setFloat("light.quadratic", 0.032f);

		buddhaModel.Draw(shader);
	}

private:
	unsigned int VAO, VBO;
	unsigned int cubemapTexture;
	Model buddhaModel;
};