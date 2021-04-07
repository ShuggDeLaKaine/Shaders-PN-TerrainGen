#pragma once

#include <glad/glad.h> 
#include <vector>
#include <set>
#include <iostream>
#include <glm/glm.hpp>

using namespace std;

/*	\class Isco
*	\brief Creates a 20 sided geometric shape (iscohederon) with 1 PER VERTEX normals
*/
class Icso {

private:
	const float X = -0.52;
	const float Z = -0.85;
	const int numVerts = 12;
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	glm::vec3 getNormal(vector<int> face);
	void makePositions();

public:
	Icso();
	void getVertices(int* indices, int numInd, float* vertices, int numVerts);
};