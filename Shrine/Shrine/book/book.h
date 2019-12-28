#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../shader.h"

//using namespace std;
float bookWidth = 2.04f;
float bookHeight = 3.16f;

float bookVertices[] = {
	bookWidth, bookHeight, 0.0f, 1.0f, 1.0f,   // 右上
	0.0f, bookHeight, 0.0f, 0.0f, 1.0f,    // 左上
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // 左下

	bookWidth, 0.0f, 0.0f, 1.0f, 0.0f,   // 右下
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // 左下
	bookWidth, bookHeight, 0.0f, 1.0f, 1.0f,   // 右上
};

class Book {
public:
	Shader bookShader;

	Book()
		: bookShader(Shader("book/book.vs", "book/book.fs")), bookPosZ(35.0f)
	{
		// eightDiagram VAO
		glGenVertexArrays(1, &bookVAO);
		glGenBuffers(1, &bookVBO);
		glBindVertexArray(bookVAO);
		glBindBuffer(GL_ARRAY_BUFFER, bookVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bookVertices), &bookVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// load textures
		// -------------
		bookTexture = loadTexture("resources/textures/book.jpg");
		normMapTexture = loadTexture("resources/textures/book-norm.jpg");

		//bookShader.use();
		//bookShader.setInt("texture1", 0);
		//bookShader.setInt("texture1", 0);
	}

	void drawBook(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera camera) {
		bookShader.use();
		bookShader.setMat4("model", model);
		bookShader.setMat4("view", view);
		bookShader.setMat4("projection", projection);
		bookShader.setInt("material.diffuse", 0);
		bookShader.setInt("material.specular", 1);
		bookShader.setInt("normalMap", 2);
		bookShader.setFloat("material.shininess", 32.0f);
		bookShader.setVec3("light.position", camera.Position);
		bookShader.setVec3("light.color", 1.0f, 1.0f, 1.0f);
		//bookShader.setVec3("light.direction", 0.0f, -1.0f, 0.0f);
		bookShader.setVec3("light.direction", glm::sin((float)glfwGetTime() * 2.0f), glm::cos((float)glfwGetTime() * 2.0f), 0.0f);
		bookShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		bookShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		bookShader.setVec3("viewPos", camera.Position);
		float ambientValue = 0.3f;
		float diffuseValue = 1.0f;
		float specularValue = 1.0f;
		bookShader.setVec3("light.ambient", ambientValue, ambientValue, ambientValue);
		bookShader.setVec3("light.diffuse", diffuseValue, diffuseValue, diffuseValue);
		bookShader.setVec3("light.specular", specularValue, specularValue, specularValue);
		bookShader.setFloat("light.constant", 1.0f);
		bookShader.setFloat("light.linear", 0.09f);
		bookShader.setFloat("light.quadratic", 0.032f);
		bookShader.setFloat("cameraPos", camera.Position.z);

		glBindVertexArray(bookVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bookTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bookTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 24);
		glBindVertexArray(0);
	}

	float getPosZ() { return this->bookPosZ; }

	void setPosZ(float posZ) { this->bookPosZ = posZ; }

	float getVZ() { 
		float diff = glm::pow(40.0f - this->bookPosZ, 1.3);

		return 0.05f * diff; 
	}

private:
	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	unsigned int bookVAO, bookVBO;
	unsigned int bookTexture;
	unsigned int normMapTexture;
	float bookPosZ;

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