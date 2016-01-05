/*************************************************

myShader.h

copied from GLSL CookBook

*************************************************/

#ifndef MYSHADER_H
#define MYSHADER_H
//std libraries
#include <string>

//gl libraries
#include <GL/glew.h>
#include <glm.hpp>

namespace GLSLShader{
	enum GLSLShaderType{
		VERTEX, 
		FRAGMENT, 
		GEOMETRY, 
		TESS_CONTROL,
		TESS_EVALUATION
	};
};

class myShader
{
public:
	myShader();
	
	bool compileShader(const char *filename, GLSLShader::GLSLShaderType type);
	bool compileShader(const std::string &source, GLSLShader::GLSLShaderType type);
	bool link();
	void use();
	
	std::string log();
	
	int getHandle();
	bool isLinked();
	
	void bindAttribLocation(GLuint location, const char *name);
	void bindFragDataLocation(GLuint location, const char *name);

	void setUniform(const char *name, float x, float y, float z);
	void setUniform(const char *name, const glm::vec3 &v);
	void setUniform(const char *name, const glm::vec4 &v);
	void setUniform(const char *name, const glm::mat4 &m);
	void setUniform(const char *name, const glm::mat3 &m);
	void setUniform(const char *name, float val);
	void setUniform(const char *name, int val);
	void setUniform(const char *name, bool val);

	void printActiveUniforms();
	void printActiveAttribs();
	
private:
	int handle;
	bool linked;
	std::string logString;
	
	int getUniformLocation(const char* name);
	bool fileExists(const std::string &filename);
};

#endif