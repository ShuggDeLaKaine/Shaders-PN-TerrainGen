#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

#include <Model.h>
#include "Terrain.h"

#include <string>
#include <iostream>
#include <numeric>


//settings.
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

//forward declared functions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);
void setVAO(vector <float> vertices);
void processMVPdata(Shader shader, glm::mat4 proj, glm::mat4 vw, glm::mat4 mod);
void processCameraPosition(Shader shader, glm::vec3 position);

//camera.
//Camera camera(glm::vec3(350.0f, 270.0f, 510.0f));
//Camera camera(glm::vec3(20.0f, 645.0f, 515.0f));
//Camera camera(glm::vec3(275.0f, 20.0f, 500.0f));
Camera camera(glm::vec3(100.0f, 320.0f, 450.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//lighting.
glm::vec3 dirLightPos(0.4f, 0.6f, 0.5f);			//direction light is shining in; think of 0-1, experiment with this.

//arrays.
unsigned int terrainVAO, VBO, VAO, EBO;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
#pragma region INITIALISATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
#pragma endregion

#pragma region SHADERS
	Shader HeightMapMShader("..\\shaders\\vert\\HMVert.vs", "..\\shaders\\frag\\HMFrag.fs", "..\\shaders\\geo\\geoHM.gs",
						"..\\shaders\\tess\\control\\tessHMControl.tcs", "..\\shaders\\tess\\eval\\tessHMEval.tes");

	Shader TerrainShader("..\\shaders\\vert\\HMVert.vs", "..\\shaders\\frag\\TerrainFrag.fs", "..\\shaders\\geo\\geoHM.gs",
						"..\\shaders\\tess\\control\\tessHMControl.tcs", "..\\shaders\\tess\\eval\\tessHMEval.tes");

	Shader PerlinShader("..\\shaders\\vert\\HMVert.vs", "..\\shaders\\frag\\TerrainFrag.fs", "..\\shaders\\geo\\geoPerlin.gs",
						"..\\shaders\\tess\\control\\tessPerlinControl.tcs", "..\\shaders\\tess\\eval\\tessPerlinEval.tes");

#pragma endregion

	unsigned int basicHM = loadTexture("..\\resources\\heightMaps\\heightMap.png");				//good.
	//unsigned int moonCrater01 = loadTexture("..\\resources\\heightMaps\\MoonCrater01.jpg");	//not the best.
	//unsigned int moonCrater02 = loadTexture("..\\resources\\heightMaps\\MoonCrater02.png");	//not good.
	//unsigned int valley = loadTexture("..\\resources\\heightMaps\\valleyHM.png");				//alright.

	unsigned int grassTexture = loadTexture("..\\resources\\textures\\grass03.png");
	unsigned int rockTexture = loadTexture("..\\resources\\textures\\rock01.jpg");
	unsigned int dirtTexture = loadTexture("..\\resources\\textures\\dirt04.jpg");
	unsigned int soilTexture = loadTexture("..\\resources\\textures\\soil01.jpg");
	unsigned int snowTexture = loadTexture("..\\resources\\textures\\snow01.jpg");


	//terrain constructor; number of grids in width, number of grids in height, gridSize.
	Terrain terrain(50, 50, 10);
	terrainVAO = terrain.getVAO();

	
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		const glm::vec3 sky(0.4f, 0.7f, 0.8f);

		glClearColor(sky.r, sky.g, sky.b, 1.0f);
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2500.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		//ALL THE STUFF FOR A HEIGHT MAP SHADER.
		/*
		//set model, view, projection and camera data.
		TerrainShader.use();
		TerrainShader.setMat4("projection", projection);
		TerrainShader.setMat4("view", view);
		TerrainShader.setMat4("model", model);
		TerrainShader.setVec3("viewPos", camera.Position);

		TerrainShader.setVec3("sky", sky);
		TerrainShader.setInt("scale", 250);
		TerrainShader.setBool("useFog", false);
		TerrainShader.setBool("useTextures", false);

		//set the textures and active/bind them.
		//TerrainShader.setInt("heightMap", 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, basicHM);

		TerrainShader.setInt("grassTexture", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		TerrainShader.setInt("rockTexture", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rockTexture);
		TerrainShader.setInt("dirtTexture", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, dirtTexture);
		TerrainShader.setInt("soilTexture", 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, soilTexture);
		TerrainShader.setInt("snowTexture", 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, snowTexture);

		//light properties.
		TerrainShader.setVec3("dirLight.direction", dirLightPos);
		TerrainShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		TerrainShader.setVec3("dirLight.diffuse", 0.55f, 0.55f, 0.55f);
		TerrainShader.setVec3("dirLight.specular", 0.6f, 0.6f, 0.6f);

		//material properties.
		TerrainShader.setVec3("mat.ambient", 0.3f, 0.4f, 0.3f);
		TerrainShader.setVec3("mat.diffuse", 0.4f, 0.7f, 0.7f);
		TerrainShader.setVec3("mat.specular", 0.3f, 0.3f, 0.3f);
		TerrainShader.setFloat("mat.shininess", 0.9f);
		*/

		//PERLIN NOISE shader stuff.
		//set model, view, projection and camera data.
		PerlinShader.use();
		PerlinShader.setMat4("projection", projection);
		PerlinShader.setMat4("view", view);
		PerlinShader.setMat4("model", model);
		PerlinShader.setVec3("viewPos", camera.Position);

		PerlinShader.setVec3("sky", sky);
		PerlinShader.setInt("scale", 450);
		PerlinShader.setInt("octaves", 10);
		PerlinShader.setFloat("minTessLvl", 1.0f);

		PerlinShader.setBool("useFog", true);
		PerlinShader.setBool("useJustTextures", false);
		PerlinShader.setBool("useTexturesAndColour", true);
		PerlinShader.setBool("useLowPolyFlatSurfaces", true);
		PerlinShader.setBool("perlinOnGPU", true);
		

		PerlinShader.setInt("grassTexture", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		PerlinShader.setInt("rockTexture", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rockTexture);
		PerlinShader.setInt("dirtTexture", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, dirtTexture);
		PerlinShader.setInt("soilTexture", 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, soilTexture);
		PerlinShader.setInt("snowTexture", 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, snowTexture);

		//light properties.
		PerlinShader.setVec3("dirLight.direction", dirLightPos);
		PerlinShader.setVec3("dirLight.ambient", 0.4f, 0.4f, 0.4f);
		PerlinShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
		PerlinShader.setVec3("dirLight.specular", 0.35f, 0.35f, 0.35f);

		//material properties.
		PerlinShader.setVec3("mat.ambient", 0.3f, 0.4f, 0.3f);
		PerlinShader.setVec3("mat.diffuse", 0.4f, 0.7f, 0.7f);
		PerlinShader.setVec3("mat.specular", 0.3f, 0.3f, 0.3f);
		PerlinShader.setFloat("mat.shininess", 0.45f);
		


		glBindVertexArray(terrainVAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_PATCHES, 0, terrain.getSize());

		//testing purposes, print position of camera.
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			camera.printCameraCoords();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



//function to set the camera position to a shader
void processCameraPosition(Shader shader, glm::vec3 position)
{
	shader.setVec3("cameraPosition", position);
}

//function to sort the Model, View and Projection data with the required shader.
void processMVPdata(Shader shader, glm::mat4 proj, glm::mat4 vw, glm::mat4 mod)
{
	shader.use();
	shader.setMat4("projection", proj);
	shader.setMat4("view", vw);
	shader.setMat4("model", mod);
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly.
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(UPWARDS, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWNWARDS, deltaTime);
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//glfw: whenever the mouse moves, this callback is called.
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw: whenever the mouse scroll wheel scrolls, this callback is called.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

	//if there is data there...
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
		//create texture and generate mipmaps.
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//set texture wrapping parameters, in this case to GL_MIRRORED_REPEAT.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		//set texture filtering parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);
		std::cout << "Loaded texture at path: " << path << " width " << width << " id " << textureID << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

//function to set the VAO.
void setVAO(vector <float> vertices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);
	//x, y, z.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//textures.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//bind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}