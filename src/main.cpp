#include <cmath>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <iostream>

#include <glad/glad.h> // OpenGL function pointers resolution 
#include <GLFW/glfw3.h> //Extensionns on openGL functionality to interact with OS 
#include <sstream>


//Define vertex data
float vertexData[]= {
	-0.5f, -0.5f, 0.0f,	
	 0.5f,  0.5f, 0.0f,	
	 0.0f,  0.5f, 0.0f,	
};

float vertex2[]= {
	-0.5f, -0.5f, 0.0f,	
	 0.5f,  0.5f, 0.0f,	
	 0.0f,  0.5f, 0.0f,	
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  
void checkForEscInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* setupGlfwWindow(){

	//Open GL definitions for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//window setup
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	return window;
}

void setupCallbacks(GLFWwindow* window){
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //resize callback function
}

std::string fetchShaderSource(std::string path){
	std::ifstream fs(path);
	std::stringstream fss;
	fss << fs.rdbuf();
	std::string fileContents = fss.str();
	return fileContents;
}

void cleanup(){
	glfwTerminate();
}

int main() {
	glfwInit(); //initialize glfw

	GLFWwindow* window = setupGlfwWindow(); 
	if (window == NULL) return -1;

	//Create OpenGL Context
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
	}
	
	//Viewport setup. This is somehow different from 
	//glfwCreateWindow, but I`m not so sure how different
	//it is
	glViewport(0,0,800,600);

	//Callbacks definition
	setupCallbacks(window);
	
	std::string vertSrc = fetchShaderSource("./src/vertex_shader.glsl");
	std::string fragSrc = fetchShaderSource("./src/frag_shader.glsl");
	const char* vertexShaderSrc = vertSrc.c_str();
	const char* fragShaderSrc = fragSrc.c_str();


	int success;
	char infoLog[512];
	//Create shaders
	glCreateShader(GL_VERTEX_SHADER);
	uint32_t vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	uint32_t frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	//c_str terminates the string with null terminator. This way the last argument dont
	//need to be set 
	glShaderSource(vert_shader_id, 1, &vertexShaderSrc, NULL);
	glShaderSource(frag_shader_id, 1, &fragShaderSrc, NULL);

	glCompileShader(vert_shader_id);
	glCompileShader(frag_shader_id); 
	glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
			glGetShaderInfoLog(vert_shader_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
			glGetShaderInfoLog(frag_shader_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//c_str terminates the string with null terminator. This way the last argument dont
	//need to be set 
	uint32_t programId = glCreateProgram();
	glAttachShader(programId,vert_shader_id);
	glAttachShader(programId,frag_shader_id);
	glLinkProgram(programId);
	glUseProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vert_shader_id);
	glDeleteShader(frag_shader_id);

	//Create BufferObject to store vertex data
	int bufferObj,vao;
	glGenBuffers(1, (GLuint*)&bufferObj);

	glGenVertexArrays(1,(GLuint*)&vao);
	glBindVertexArray(vao);

	//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
	glBindBuffer(GL_ARRAY_BUFFER, bufferObj); 
	//Notice we can transfer data for the buffer bind point, but not directly
	//to the buffer itself. Its kind of a proxy
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	


	//render loop: Each loop is a frame 
	while(!glfwWindowShouldClose(window)) {
			checkForEscInput(window);

			glClearColor(0.5f,0.5f,0.5f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(programId);

			float timeDiff = glfwGetTime();
			float greenVariance = (sin(timeDiff)/2.0f) + 0.5f;
			int uniformAddr = glGetUniformLocation(programId,"changingColor");
			glUniform4f(uniformAddr, 1.0, greenVariance, 0.5,1.0);

			glBindVertexArray(vao);

			glDrawArrays(GL_TRIANGLES, 0, 3);
			
			//All the operations from this frame where executed on the back buffer. The next frame
			//will show those alterations and move the actual front buffer to the back, so we can change it 
			glfwSwapBuffers(window); 
			glfwPollEvents(); 
	}
	cleanup();

	return 0;
}
