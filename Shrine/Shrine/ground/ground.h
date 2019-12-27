#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../shader.h"
#include "../camera.h"

const float groundSize = 100.0f;
const float groundVertices[] = {
	groundSize, 0.0f, -groundSize, 1.0f, 1.0f,   // 右上
	-groundSize, 0.0f, -groundSize, 0.0f, 1.0f,    // 左上
	-groundSize, 0.0f, groundSize, 0.0f, 0.0f,   // 左下

	groundSize, 0.0f, groundSize, 1.0f, 0.0f,   // 右下
	-groundSize, 0.0f, groundSize, 0.0f, 0.0f,   // 左下
	groundSize, 0.0f, -groundSize, 1.0f, 1.0f,   // 右上
};

class Ground {
public:
	Shader shader;

	Ground()
		: shader(Shader("ground/ground.vs", "ground/ground.fs"))
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), &groundVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// load textures
		// -------------
		texture = loadTexture("resources/morose/morose_dn.jpg");
		//normMapTexture = loadTexture("resources/textures/book-norm.jpg");

		//bookShader.use();
		//bookShader.setInt("texture1", 0);
		//bookShader.setInt("texture1", 0);
	}

	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	unsigned int VAO, VBO;
	unsigned int texture;
	//unsigned int normMapTexture;

	unsigned int loadTexture(char const * path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);  // need this
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
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
};