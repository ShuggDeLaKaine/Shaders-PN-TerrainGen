#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

/*
   This class, at the moment, doesnt do a lot.
   It will create a VAO for the 'floor' ( 4 points) and also a VAO for an array of 5 points at which you can create billboards.
   You can extend this to do more interesting things if you want.
   The public functions return the VAO ids (unsigned ints).
*/

class Billboard {

private:
	int floorSize;
	int numPoints;

public:
	Billboard();
	unsigned int getPoints();
	unsigned int getFloor();
};