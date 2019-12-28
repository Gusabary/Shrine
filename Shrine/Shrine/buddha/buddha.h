#pragma once
#include <vector>
#include "../shader.h"
#include "../model.h"
#include "../camera.h"

using namespace std;

class Buddha {
public:
	Shader shader;
	bool startGlare;

	Buddha()
		: shader(Shader("buddha/buddha.vs", "buddha/buddha.fs")),
		  buddhaModel(Model("resources/objects/buddha/12334_statue_v1_l3.obj")),
		  startGlare(false), glareValue(0.0f)
	{}

	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		shader.use();
		shader.setVec3("cameraPos", camera.Position);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);
		shader.setFloat("material.shininess", 51.0);
		shader.setVec3("light.position", camera.Position);
		shader.setVec3("light.color", 1.0f, 1.0f, 1.0f);
		shader.setVec3("light.direction", camera.Front);
		shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("light.outerCutOff", glm::cos(glm::radians(30.0f)));
		shader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.3f;
		float diffuseValue = camera.IsLightOn ? getDiffuseLightValue() : 0.0f;  // 0.5~5.0
		float specularValue = camera.IsLightOn ? getSpecularLightValue() : 0.0f;  // 0.5~3.0
		shader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		shader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		shader.setVec3("light.specular", specularValue, specularValue, specularValue);
		shader.setFloat("light.constant", 1.0f);
		shader.setFloat("light.linear", getLinearValue());  // 0.09~0.01
		shader.setFloat("light.quadratic", 0.032f);

		shader.setVec3("cover.ambient", getAmbientCover());
		shader.setVec3("cover.diffuse", getDiffuseCover());
		shader.setVec3("cover.specular", getSpecularCover());

		buddhaModel.Draw(shader);
	}

	void accumulateGlare(float deltaTime) {
		if (!this->startGlare)
			return;
		glareValue += deltaTime;  // whole process should complete in 10 seconds
		if (glareValue > 10.0f)
			glareValue = 10.0f;
	}

private:
	unsigned int VAO, VBO;
	unsigned int cubemapTexture;
	Model buddhaModel;
	float glareValue;
	const glm::vec3 bronzeAmbient = glm::vec3(0.212500, 0.127500, 0.054000);
	const glm::vec3 bronzeDiffuse = glm::vec3(0.714000, 0.428400, 0.181440);
	const glm::vec3 bronzeSpecular = glm::vec3(0.393548, 0.271906, 0.166721);
	const glm::vec3 silverAmbient = glm::vec3(0.192250, 0.192250, 0.192250);
	const glm::vec3 silverDiffuse = glm::vec3(0.507540, 0.507540, 0.507540);
	const glm::vec3 silverSpecular = glm::vec3(0.508273, 0.508273, 0.508273);
	const glm::vec3 goldAmbient = glm::vec3(0.247250, 0.199500, 0.074500);
	const glm::vec3 goldDiffuse = glm::vec3(0.751640, 0.606480, 0.226480);
	const glm::vec3 goldSpecular = glm::vec3(0.628281, 0.555802, 0.366065);

	float getDiffuseLightValue() {
		return 0.45 * glareValue + 0.5;
	}

	float getSpecularLightValue() {
		return 0.25 * glareValue + 0.5;
	}

	float getLinearValue() {
		return 0.09 - 0.008 * glareValue;
	}

	glm::vec3 getAmbientCover() {
		if (glareValue < 5.0f) {
			return (glareValue / 5.0f) * (silverAmbient - bronzeAmbient) + bronzeAmbient;
		}
		return ((glareValue - 5.0f) / 5.0f) * (goldAmbient - silverAmbient) + silverAmbient;
	}

	glm::vec3 getDiffuseCover() {
		if (glareValue < 5.0f) {
			return (glareValue / 5.0f) * (silverDiffuse - bronzeDiffuse) + bronzeDiffuse;
		}
		return ((glareValue - 5.0f) / 5.0f) * (goldDiffuse - silverDiffuse) + silverDiffuse;
	}

	glm::vec3 getSpecularCover() {
		if (glareValue < 5.0f) {
			return (glareValue / 5.0f) * (silverSpecular - bronzeSpecular) + bronzeSpecular;
		}
		return ((glareValue - 5.0f) / 5.0f) * (goldSpecular - silverSpecular) + silverSpecular;
	}
};