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
		buddhaModel.Draw(shader);
	}

private:
	unsigned int VAO, VBO;
	unsigned int cubemapTexture;
	Model buddhaModel;
};