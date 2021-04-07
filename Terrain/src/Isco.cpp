#include "Isco.h"

Icso::Icso() 
{
	makePositions();
}

void Icso::makePositions() 
{
	positions.push_back(glm::vec3(-X, 0, Z));
	positions.push_back(glm::vec3(X, 0, Z));
	positions.push_back(glm::vec3(-X, 0, -Z));
	positions.push_back(glm::vec3(X, 0, -Z));

	positions.push_back(glm::vec3(0, Z, X));
	positions.push_back(glm::vec3(0, Z, -X));
	positions.push_back(glm::vec3(0, -Z, X));
	positions.push_back(glm::vec3(0, -Z, -X));

	positions.push_back(glm::vec3(Z, X, 0));
	positions.push_back(glm::vec3(-Z, X, 0));
	positions.push_back(glm::vec3(Z, -X, 0));
	positions.push_back(glm::vec3(-Z, -X, 0));
}

// get Vertices AND single per vertex normal (one normal per vertex)
// one normal is average of all normals at that vertex
// Will become clear why we need this with PN-Triangles in later labs

	// getVertices() : look through indices in triplets
	// is thisVert in indices triplet?
	// if so get surface normal of a b c
	// add this normal to running total
	// normalise running total
	// add this value to list of normals
void Icso::getVertices(int* indices, int numIndices, float* vertices, int numVerts) 
{
	glm::vec3 runningTotal;
	for (int thisVert = 0; thisVert < numVerts; thisVert++) {
		runningTotal = glm::vec3(0.0);
		for (int i = 0; i < (numIndices - 3); i += 3) {
			std::vector<int> face;
			face.push_back(indices[i]);
			face.push_back(indices[i + 1]);
			face.push_back(indices[i + 2]);
			if (std::count(face.begin(), face.end(), thisVert)) {  // if this face contains this vertex 
				runningTotal += getNormal(face);  //update running total
			}
		}
		normals.push_back(glm::normalize(runningTotal));  // normalise running total - this is the vertex normal 
	}

	// now add positions and normals to vertices[] array to use for VAO
	int counter = 0;
	for (int i = 0; i < numVerts; i++) {
		//position
		vertices[counter] = positions.at(i).x;
		counter++;
		vertices[counter] = positions.at(i).y;
		counter++;
		vertices[counter] = positions.at(i).z;
		counter++;
		//normals
		vertices[counter] = normals.at(i).x;
		counter++;
		vertices[counter] = normals.at(i).y;
		counter++;
		vertices[counter] = normals.at(i).z;
		counter++;
	}
}

//get surface normal.
glm::vec3 Icso::getNormal(vector<int> face) 
{
	glm::vec3 a = glm::vec3(positions.at(face.at(1)) - positions.at(face.at(0)));
	glm::vec3 b = glm::vec3(positions.at(face.at(2)) - positions.at(face.at(0)));
	return glm::normalize(glm::cross(a, b));
}