#include "Billboard.h"


Billboard::Billboard() {
	// don't actually need these member vars at the moment
	floorSize = 5;
	numPoints = 4;
}

unsigned int Billboard::getPoints() {
	float points[] = {
	-0.5f, 0.0f, 0.0f,
	 0.5f, 0.0f, 1.0f,
	 0.0f, 0.0f, 0.5f,
	 0.5f, 0.0f, 0.0f,
	-0.5f, 0.0f, 1.0f
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);

	return VAO;
}

unsigned int Billboard::getFloor() 
{
	float points[] = {
		 floorSize, 0.0f,  floorSize,
		-floorSize, 0.0f,  floorSize,
		-floorSize, 0.0f, -floorSize,

		 floorSize, 0.0f,  floorSize,
		-floorSize, 0.0f, -floorSize,
		 floorSize, 0.0f, -floorSize
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);

	return VAO;
}