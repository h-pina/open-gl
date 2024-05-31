#include <cmath>
#include <ostream>
#include <string>
#include <iostream>

#include "Shader.hpp"

#include <glad/glad.h> // OpenGL function pointers resolution 
#include <GLFW/glfw3.h> //Extensionns on openGL functionality to interact with OS 


//Define vertex data
float vertexData[] = {
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 1.0f    // top 
};    
float v2D[] = {
    -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f    // top 
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
	
	Shader shaderProgram = Shader("./src/vertex_shader.glsl","./src/frag_shader.glsl");

	//Create BufferObject to store vertex data
	int bufferObjs[2], vaos[2];
	glGenBuffers(2, (GLuint*)&bufferObjs);
	glGenVertexArrays(2,(GLuint*)&vaos);

	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjs[0]); 
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjs[1]); 
	glBufferData(GL_ARRAY_BUFFER,sizeof(v2D),v2D,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//render loop: Each loop is a frame 
	while(!glfwWindowShouldClose(window)) {
			checkForEscInput(window);

			glClearColor(0.5f,0.5f,0.5f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();

			glBindVertexArray(vaos[0]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			
			glBindVertexArray(vaos[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			//All the operations from this frame where executed on the back buffer. The next frame
			//will show those alterations and move the actual front buffer to the back, so we can change it 
			glfwSwapBuffers(window); 
			glfwPollEvents(); 
	}
	cleanup();

	return 0;
}
