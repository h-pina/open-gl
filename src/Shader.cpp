#include "Shader.hpp"
#include "Logger.hpp"
#include <sstream>
#include <stdexcept>
#include "fstream"
#include "string"
#include "sstream"
#include "iostream"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertex_src_path, const char* frag_src_path){
	Logger logger;
	std::string vert_src_str = fetchSourceCode(vertex_src_path);
	std::string frag_src_str = fetchSourceCode(frag_src_path);
	const char* vertex_source = vert_src_str.c_str();
	const char* frag_source = frag_src_str.c_str();

	int success;
	char errorLog[250];

	int vert_id;
	vert_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_id,1,&vertex_source, NULL);
	glCompileShader(vert_id);
	glGetShaderiv(vert_id,GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE){
		glGetShaderInfoLog(vert_id, 250, NULL, errorLog);
		std::stringstream errormsg;
		errormsg << "Vertex Shader compilation error: " << errorLog << std::endl;
		logger.error(errormsg.str().c_str());
		glDeleteShader(vert_id); 
		throw std::runtime_error(errorLog);
	}

	GLuint frag_id;
	frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_id,1,&frag_source, NULL);
	glCompileShader(frag_id);
	glGetShaderiv(frag_id,GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE){
		glGetShaderInfoLog(frag_id, 250, NULL, errorLog);
		std::stringstream errormsg;
		errormsg << "Fragment Shader compilation error: " << errorLog << std::endl;
		logger.error(errormsg.str().c_str()); //dangling pointer possible error
		glDeleteShader(frag_id); 
		throw std::runtime_error(errorLog);
	}

	programId = glCreateProgram();
	glAttachShader(programId,vert_id);
	glAttachShader(programId,frag_id);
	glLinkProgram(programId);
	glGetProgramiv(frag_id,GL_LINK_STATUS, &success);
	if(success == GL_FALSE){
		glGetProgramInfoLog(frag_id, 250, NULL, errorLog);
		std::stringstream errormsg;
		errormsg << "Program linking error: " << errorLog << std::endl;
		logger.error(errormsg.str().c_str());
		throw std::runtime_error(errorLog);
	}

	glDeleteShader(vert_id);
	glDeleteShader(frag_id);
}
void Shader::use(){
	glUseProgram(programId);
}

std::string Shader::fetchSourceCode(const char* path){
	try{
		std::ifstream fs;
		fs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		fs.open(path);
		std::stringstream fss;
		fss << fs.rdbuf();
		return  fss.str();
	}
	catch(std::ifstream::failure* e){
		std::cout << "Error opening file " << path << std::endl;	
		throw std::invalid_argument("Could not open shader file");
	}
}

void Shader::setMat4Uniform(const char* name, glm::mat4 &value){
	GLuint uniformRef = glGetUniformLocation(programId,name);
	glUniformMatrix4fv(uniformRef, 1,GL_FALSE,glm::value_ptr(value));
}

