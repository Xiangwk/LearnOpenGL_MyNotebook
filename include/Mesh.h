#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <Shader.h>

#include <vector>
#include <string>

#include <Texture2D.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 biTangent;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture2D> textures;
	std::vector<GLuint> indices;

	GLuint VAO, VBO, EBO;

	Mesh(const std::vector<Vertex> &v, const std::vector<Texture2D> &t = {}, const std::vector<GLuint> &i = {});
	void draw(Shader shader) const;

private:
	void setupVAO();
};

Mesh::Mesh(const std::vector<Vertex> &v, const std::vector<Texture2D> &t, const std::vector<GLuint> &i) :
vertices(v), textures(t), indices(i)
{
	setupVAO();
}

void Mesh::draw(Shader shader) const
{
	unsigned diffNum = 1, specNum = 1, reflectNum = 1, normalNum = 1;
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		int n = 0;
		std::string t = textures[i].type;
		if (!t.empty())
		{
			//we use n to judge the texture's name in shader
			//in shader we must named textures as "texture_diffuseN" or "texture_specularN" or others(N is start from 1)
			if (t == "texture_diffuse")
				n = diffNum++;
			else if (t == "texture_specular")
				n = specNum++;
			else if (t == "texture_reflect")
				n = reflectNum++;
			else if (t == "texture_normal")
				n = normalNum++;
			std::string N = std::to_string(n);
			//don't forget "material.", in shader the sampler of texture is in Material structure
			shader.setUniformInt("material." + t + N, i);
		}
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	if (!indices.empty())
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupVAO()
{
	if (vertices.empty())
	{
		std::cerr << "Vertices load failed!" << std::endl;
		return;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	if (!indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}