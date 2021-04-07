#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "PerlinNoise.h"


class Terrain
{
public:
	Terrain(int widthIn, int heightIn, int stepSizeIn);
	Terrain();
	double cycleOctaves(glm::vec3 position, int numOctaves);
	unsigned int getVAO();
	int getSize();
protected:
	std::vector<float> getVertices();
private:
	PerlinNoise perlin;
	std::vector<float> vertices;
	unsigned int VAO, VBO;
	int width;
	int height;
	int stepSize;
	void makeVertices(std::vector<float> *vertices);
	void makeVertex(int x, int y, std::vector<float> *vertices);
};
#endif
