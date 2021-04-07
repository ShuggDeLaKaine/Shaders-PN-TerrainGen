
#include <shader.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessControlPath, const char* tessEvalPath)
{
	//retrieve the vertex/fragment/geometry source code from filePath.
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::string tessControlCode;
	std::string tessEvalCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	std::ifstream tcShaderFile;
	std::ifstream teShaderFile;

	//ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open files.
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//read files buffer contents into streams.
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//close file handlers.
		vShaderFile.close();
		fShaderFile.close();

		//convert stream into string.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		//check that the tessellation control pathfile isn't null and then do the same as above for the tessellation control shader.
		if (tessControlPath != nullptr)
		{
			tcShaderFile.open(tessControlPath);
			std::stringstream tcShaderStream;
			tcShaderStream << tcShaderFile.rdbuf();
			tcShaderFile.close();
			tessControlCode = tcShaderStream.str();
		}

		//check that the tessellation evaulation pathfile isn't null and then do the same as above for the tessellation evaulation shader.
		if (tessEvalPath != nullptr)
		{
			teShaderFile.open(tessEvalPath);
			std::stringstream teShaderStream;
			teShaderStream << teShaderFile.rdbuf();
			teShaderFile.close();
			tessEvalCode = teShaderStream.str();
		}

		//check that the geometry pathfile isn't null and then do the same as above for the geometry shader.
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	//then compile the shaders.
	unsigned int vertex, fragment, geometry, tessCon, tessEval;

	//create vertex shader.
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	//create fragment shader.
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	//check that the geometry pathfile isn't null and then do the same as above, creating the geometry shader.
	if (tessControlPath != nullptr)
	{
		const char* tcShaderCode = tessControlCode.c_str();
		tessCon = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessCon, 1, &tcShaderCode, NULL);
		glCompileShader(tessCon);
		checkCompileErrors(tessCon, "TESSELLATION CONTROL");
	}

	//check that the geometry pathfile isn't null and then do the same as above, creating the geometry shader.
	if (tessEvalPath != nullptr)
	{
		const char* teShaderCode = tessEvalCode.c_str();
		tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessEval, 1, &teShaderCode, NULL);
		glCompileShader(tessEval);
		checkCompileErrors(tessEval, "TESSELLATION EVALUATION");
	}

	//check that the geometry pathfile isn't null and then do the same as above, creating the geometry shader.
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	//create shader program.
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	//same ol' tessellation and geometry checks.
	if (tessControlPath != nullptr)
		glAttachShader(ID, tessCon);
	if (tessEvalPath != nullptr)
		glAttachShader(ID, tessEval);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	//delete the shaders as they're linked into our program now and no longer necessery.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	//same ol' tessellation and geometry checks.
	if (tessControlPath != nullptr)
		glDeleteShader(tessCon);
	if (tessEvalPath != nullptr)
		glDeleteShader(tessEval);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}

// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

