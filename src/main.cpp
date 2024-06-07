

#include "Logger.hpp"
#include "Shader.hpp"

#include <glad/glad.h> // OpenGL function pointers resolution 
#include <GLFW/glfw3.h> //Extensionns on openGL functionality to interact with OS 
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>

#include "stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


Logger logger; //todo: change this from global obj

//Define vertex data
float vertices[] = {
		//vert positions			//texture data 
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
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
		logger.error("Failed to create GLFW window"); 
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

glm::mat4 initCamera(){
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	return view;
}

glm::mat4 spinCamera(){ //Maybe this function should consider view previous statek
	glm::mat4 view = glm::mat4(1.0f);
	return view;
}


int main() {
	logger.debug("Intializing glfw");
	glfwInit(); //initialize glfw
	logger.debug("Done");

	logger.debug("Setting up glfw window");
	GLFWwindow* window = setupGlfwWindow(); 
	if (window == NULL) return -1;
	logger.debug("Done");

	//Create OpenGL Context
	logger.debug("Setting up OpenGL context");
	glfwMakeContextCurrent(window);
	logger.debug("Done");
	logger.debug("Loading OpenGL function pointers using GLAD");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger.error("Failed to initialize GLAD");
		return -1;
	}
	logger.debug("Done");
	
	//Viewport setup. This is somehow different from 
	//glfwCreateWindow, but I`m not so sure how different
	//it is
	glViewport(0,0,800,600);

	//Callbacks definition
	logger.debug("Setting up callbacks");
	setupCallbacks(window);
	logger.debug("Done");
	
	logger.debug("Setting up shaders ");
	Shader shaderProgram = Shader("./src/vertex_shader.glsl","./src/frag_shader.glsl");
	logger.debug("Done");

		//Create BufferObject to store vertex data
		logger.debug("Setting up VAOs and Buffer Objects");
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
		
		glBindVertexArray(0); //Unbinding after configuring
	logger.debug("Done");

	logger.debug("Setting up textures");
	GLuint woodTextureId;
	glGenTextures(1, &woodTextureId);
	glBindTexture(GL_TEXTURE_2D, woodTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texHeight, texWidth, texColorChs;
	const char* texturePath_1  = "./textures/wood.jpg";
	unsigned char* data = stbi_load(texturePath_1,  &texWidth, &texHeight, &texColorChs, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::string errormsg = "Could not load texture in path ";
		errormsg+= texturePath_1;
		logger.error(errormsg.c_str());
		throw std::invalid_argument(errormsg);
	}
	stbi_image_free(data);

	GLuint smileyFaceTextureId;
	glGenTextures(1, &smileyFaceTextureId);
	glBindTexture(GL_TEXTURE_2D, smileyFaceTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	const char* texturePath_2  = "./textures/smiley.png";
	stbi_set_flip_vertically_on_load(true);  
	data = stbi_load(texturePath_2,  &texWidth, &texHeight, &texColorChs, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::string errormsg = "Could not load texture in path ";
		errormsg+= texturePath_2;
		logger.error(errormsg.c_str());
		throw std::invalid_argument(errormsg);
	}
	stbi_image_free(data);
	logger.debug("Done");
	
	shaderProgram.use();
	//Define which texture units will the samplers read from 
	glUniform1i(glGetUniformLocation(shaderProgram.programId, "woodenSampler"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.programId, "smileySampler"), 1);

	glm::mat4 projection = glm::mat4(1.0f);
	glm:: mat4 view = initCamera();

	projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
	shaderProgram.setMat4Uniform("projection", projection); 

	//render loop: Each loop is a frame 
	logger.debug("Beggining render loop ");
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)) {
			checkForEscInput(window);
			
			glClearColor(0.5f,0.5f,0.5f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Set Texture Units 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, woodTextureId);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, smileyFaceTextureId);
			shaderProgram.use();
			for(size_t i = 0; i<10; i++){
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				view = spinCamera();
				shaderProgram.setMat4Uniform("view", view); 
				shaderProgram.setMat4Uniform("model", model); 
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			
			//Draw object
		
			glfwSwapBuffers(window); 
			glfwPollEvents(); 
	}
	logger.debug("Render loop exited. Cleaning up");
	cleanup();
	logger.debug("Cleanup done!");
	return 0;
}
