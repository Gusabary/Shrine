#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../shader.h"

//using namespace std;

float sina = glm::sin(22.5f);
float cosa = glm::cos(22.5f);
float d = 0.08f;
float e = 1.0f / (2.0f * glm::cos(22.5f));
float sine = e * sina;
float cose = e * cosa;
float eightDiagram[] = {
	-sina - d, cosa, 0.0f, -sine + d + 0.5f, cose + 0.5f,
	sina + d, cosa, 0.0f, sine - d + 0.5f, cose + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	sina + d, cosa, 0.0f, sine - d + 0.5f, cose + 0.5f,
	cosa, sina + d, 0.0f, cose + 0.5f, sine - d + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	cosa, sina + d, 0.0f, cose + 0.5f, sine - d + 0.5f,
	cosa, -sina - d, 0.0f, cose + 0.5f, -sine + d + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	cosa, -sina - d, 0.0f, cose + 0.5f, -sine + d + 0.5f,
	sina + d, -cosa, 0.0f, sine - d + 0.5f, -cose + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	sina + d, -cosa, 0.0f, sine - d + 0.5f, -cose + 0.5f,
	-sina - d, -cosa, 0.0f, -sine + d + 0.5f, -cose + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	-sina - d, -cosa, 0.0f, -sine + d + 0.5f, -cose + 0.5f,
	-cosa, -sina - d, 0.0f, -cose + 0.5f, -sine + d + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	-cosa, -sina - d, 0.0f, -cose + 0.5f, -sine + d + 0.5f,
	-cosa, sina + d, 0.0f, -cose + 0.5f, sine - d + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
	-cosa, sina + d, 0.0f, -cose + 0.5f, sine - d + 0.5f,
	-sina - d, cosa, 0.0f, -sine + d + 0.5f, cose + 0.5f,
	0.0f, 0.0f, 0.0f, 0.5f, 0.5f,
};

class EightDiagram {
public:
	Shader eightDiagramShader;

	EightDiagram()
		: eightDiagramShader(Shader("eight-diagram/eight-diagram.vs", "eight-diagram/eight-diagram.fs")),
		  pos(20.0f), rotateV(5.0f), translateV(5.0f)
	{
		// eightDiagram VAO
		glGenVertexArrays(1, &edVAO);
		glGenBuffers(1, &edVBO);
		glBindVertexArray(edVAO);
		glBindBuffer(GL_ARRAY_BUFFER, edVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(eightDiagram), &eightDiagram, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// load textures
		// -------------
		edTexture = loadTexture("resources/textures/eight-diagram.jpg");

		eightDiagramShader.use();
		eightDiagramShader.setInt("texture1", 0);
	}

	void drawEightDiagram(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
		eightDiagramShader.use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		eightDiagramShader.setMat4("model", model);
		eightDiagramShader.setMat4("view", view);
		eightDiagramShader.setMat4("projection", projection);
		
		glBindVertexArray(edVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, edTexture);
		glDrawArrays(GL_TRIANGLES, 0, 24);
		glBindVertexArray(0);
	}

	float getPos() { return this->pos; }

	void setPos(float pos) { this->pos = pos; }

	float getRotateV() { return this->rotateV; }

	float getTranslateV() { return this->translateV; }

private:
	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	unsigned int edVAO, edVBO;
	unsigned int edTexture;
	float pos;
	const float rotateV, translateV;

	unsigned int loadTexture(char const * path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
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