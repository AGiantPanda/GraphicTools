//std libraries
#include <fstream>
#include <sstream>
#include <sys/stat.h>

//gl libraries
#include "myShader.h"

myShader::myShader() : handle(0), linked(false) { }

bool myShader::compileShader(const char *filename, GLSLShader::GLSLShaderType type)
{
	if(!fileExists(filename)){
		logString = "File not exist.";
		return false;
	}
	
	if(handle<=0){
		handle = glCreateProgram();
		if(handle==0){
			logString = "Unable to create shader program.";
			return false;
		}
	}
	
	std::ifstream inFile(filename, std::ios::in);
	if(!inFile){
		return false;
	}
	
	std::ostringstream code;
	while(inFile.good()){
		int c = inFile.get();
		if(!inFile.eof()) code << (char) c;
	}
	inFile.close();
	
	return compileShader(code.str(), type);
}

bool myShader::compileShader(const std::string &source, GLSLShader::GLSLShaderType type)
{
	if(handle<=0){
		handle = glCreateProgram();
		if(handle==0){
			logString = "Unable to create shader program.";
			return false;
		}
	}
	
	GLuint shaderHandle = 0;
	
	switch(type){
	case GLSLShader::VERTEX:
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case GLSLShader::FRAGMENT:
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GLSLShader::GEOMETRY:
		shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case GLSLShader::TESS_CONTROL:
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case GLSLShader::TESS_EVALUATION:
		shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	default:
		return false;
	}
	
	const char *c_code = source.c_str();
	glShaderSource(shaderHandle, 1, &c_code, NULL);
	
	//compile the shader
	glCompileShader(shaderHandle);
	
	//check for errors
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if(GL_FALSE==result){
		//compile failed, store log and return false
		int length = 0;
		logString = "";
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		if(length>0){
			char *c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			logString = c_log;
			delete []c_log;
		}
		return false;
	}else{
		//compile succeeded, attach shader and return true
		glAttachShader(handle, shaderHandle);
		return true;
	}
}

bool myShader::link()
{
	if(linked)return true;
	if(handle<=0)return false;
	
	glLinkProgram(handle);
	
	int status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if(GL_FALSE == status){
		//store log and return false
		int length = 0;
		logString = "";
		
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
		
		if(length>0){
			char *c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(handle, length, &written, c_log);
			logString = c_log;
			delete[]c_log;
		}
		return false;
	}else{
		linked = true;
		return linked;
	}
}

void myShader::use()
{
	if(handle<=0||(!linked))return;
	glUseProgram(handle);
}

std::string myShader::log()
{
	return logString;
}

int myShader::getHandle()
{
	return handle;
}

bool myShader::isLinked()
{
	return linked;
}

void myShader::bindAttribLocation(GLuint location, const char *name)
{
	glBindAttribLocation(handle, location, name);
}

void myShader::bindFragDataLocation(GLuint location, const char *name)
{
	glBindFragDataLocation(handle, location, name);
}

void myShader::setUniform(const char *name, float x, float y, float z)
{
	int loc = getUniformLocation(name);
	if(loc>=0){
		glUniform3f(loc, x, y, z);
	}
}

void myShader::setUniform(const char *name, const glm::vec3 &v)
{
	this->setUniform(name, v.x, v.y, v.z);
}

void myShader::setUniform(const char *name, const glm::vec4 &v)
{
	int loc = getUniformLocation(name);
	if(loc>=0){
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
}

void myShader::setUniform(const char *name, const glm::mat4 &m)
{
	int loc = getUniformLocation(name);
	if(loc>=0){
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}
}

void myShader::setUniform(const char *name, const glm::mat3 &m)
{
	int loc = getUniformLocation(name);
	if(loc>=0){
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	}
}

void myShader::setUniform(const char *name, float val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ){
        glUniform1f(loc, val);
    }
}

void myShader::setUniform(const char *name, int val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ){
        glUniform1i(loc, val);
    }	
}

void myShader::setUniform(const char *name, bool val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ){
        glUniform1i(loc, val);
    }	
}

void myShader::printActiveUniforms()
{
	GLint nUniforms, size, location, maxLen;
	GLchar *name;
	GLsizei written;
	GLenum type;
	
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &nUniforms);
	
	name = (GLchar *)malloc(maxLen);
	
	printf("Location | Name\n");
	printf("--------------------------------------\n");
	for(int i=0;i<nUniforms;i++){
		glGetActiveUniform(handle, i, maxLen, &written, &size, &type, name);
		location = glGetUniformLocation(handle, name);
		printf("%-8d | %s\n",location, name);
	}
	free(name);
}

void myShader::printActiveAttribs()
{
	GLint written, size, location, maxLength, nAttribs;
	GLenum type;
	GLchar * name;

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

	name = (GLchar *)malloc(maxLength);

	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++) {
		glGetActiveAttrib(handle, i, maxLength, &written, &size, &type, name);
		location = glGetAttribLocation(handle, name);
		printf(" %-5d | %s\n", location, name);
	}

	free(name);
}

int myShader::getUniformLocation(const char *name)
{
	return glGetUniformLocation(handle, name);
}

bool myShader::fileExists(const std::string & filename)
{
	struct stat info;
	int ret = -1;
	ret = stat(filename.c_str(), &info);
	return 0 == ret;
}